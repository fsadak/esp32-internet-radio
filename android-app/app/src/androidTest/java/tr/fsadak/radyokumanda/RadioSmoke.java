package tr.fsadak.radyokumanda;

import android.app.Instrumentation;
import android.content.Intent;
import android.os.Bundle;
import java.net.HttpURLConnection;
import java.net.URL;

/** Runs only against the supplied localhost mock through emulator host gateway. */
public class RadioSmoke extends Instrumentation {
    private int checks;private boolean discoveryOnly;
    @Override public void onCreate(Bundle args){super.onCreate(args);discoveryOnly=args!=null&&"true".equals(args.getString("discovery"));start();}
    private void ok(boolean condition,String label){if(!condition)throw new AssertionError(label);checks++;}
    private void mode(String value)throws Exception {
        HttpURLConnection c=(HttpURLConnection)new URL("http://10.0.2.2/test/mode?value="+value).openConnection();
        c.setConnectTimeout(3000);c.setReadTimeout(3000);try{if(c.getResponseCode()!=200)throw new AssertionError("Mock control");}finally{c.disconnect();}
    }
    @Override public void onStart(){Bundle result=new Bundle();try{
        if(discoveryOnly){result.putString("stream",DiscoveryChecks.run(this));finish(-1,result);return;}
        mode("reset");RadioApi api=new RadioApi("10.0.2.2");RadioApi.State s=api.state();
        ok(s.volume==11,"Initial volume");ok(s.stations.size()==3,"Station list");
        ok(new RadioApi("10.0.2.2",s.deviceId).state().deviceId.equals(s.deviceId),"Expected identity");
        boolean wrong=false;try{new RadioApi("10.0.2.2","ffffffffffff").command("next",null);}catch(RadioApi.ApiError e){wrong=e.code==409;}ok(wrong,"Wrong radio identity blocked");
        s=api.command("volumeUp",null);ok(s.volume==12,"Volume up");
        s=api.command("volumeDown",null);ok(s.volume==11,"Volume down");
        s=api.command("muteOn",null);ok(s.muted,"Mute");s=api.command("muteOn",null);ok(s.muted,"Mute is idempotent");
        s=api.command("unmute",null);ok(!s.muted,"Unmute");
        s=api.command("next",null);ok(s.current==1,"Next");s=api.command("previous",null);ok(s.current==0,"Previous");
        s=api.command("select:2",s.revision);ok(s.current==2,"Select with revision");
        s=api.command("stop",null);ok(!s.playing,"Stop");s=api.command("play",null);ok(s.playing,"Play");
        mode("rotate");s=api.command("volumeUp",null);ok(s.volume==12,"403 token rotation recovery");
        String old=s.revision;mode("revision");boolean rejected=false;
        try{api.command("select:0",old);}catch(RadioApi.ApiError e){rejected=e.code==409;}ok(rejected,"Stale revision rejected");
        for(String m:new String[]{"editing","sd"}){mode(m);rejected=false;try{api.command("next",null);}catch(RadioApi.ApiError e){rejected=e.code==409;}ok(rejected,"Blocked "+m);}
        mode("badjson");rejected=false;try{api.state();}catch(java.io.IOException e){rejected=true;}ok(rejected,"Invalid state rejected");
        mode("error");rejected=false;try{api.state();}catch(RadioApi.ApiError e){rejected=e.code==503;}ok(rejected,"HTTP error preserved");
        mode("reset");
        Intent launch=new Intent(getTargetContext(),MainActivity.class);launch.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        android.app.Activity activity=startActivitySync(launch);ok(activity!=null,"Activity launches");
        waitForIdleSync();
        result.putString("stream","PASS: "+checks+" Android API/launch checks\n");finish(-1,result);
    }catch(Throwable t){result.putString("stream","FAIL after "+checks+": "+t+"\n");finish(0,result);}}
}
