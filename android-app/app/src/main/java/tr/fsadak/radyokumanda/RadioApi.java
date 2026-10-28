package tr.fsadak.radyokumanda;

import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import org.json.*;

public final class RadioApi {
    public static final class State {
        public final String revision,token,station,track,status,source,version,deviceId,deviceName;
        public final int volume,maxVolume,current;
        public final boolean muted,editing,playing;
        public final List<String> stations=new ArrayList<>();
        State(JSONObject o)throws JSONException {
            // Reject an unrelated HTTP server instead of treating it as a radio.
            token=o.getString("token");revision=o.getString("revision");
            JSONArray names=o.getJSONArray("stations");
            if(token.isEmpty()||names.length()>32)throw new JSONException("Unexpected radio state");
            for(int i=0;i<names.length();i++)stations.add(names.getString(i));
            deviceId=o.optString("deviceId");deviceName=o.optString("deviceName");
            station=o.optString("station");track=o.optString("track");status=o.optString("status");
            source=o.optString("source","radio");version=o.optString("version");
            volume=o.getInt("volume");maxVolume=o.optInt("maxVolume",21);current=o.optInt("current",0);
            muted=o.optBoolean("muted");editing=o.optBoolean("editing");playing=o.optBoolean("playing");
        }
    }
    public static final class ApiError extends IOException {
        public final int code;
        ApiError(int c,String message){super(message);code=c;}
    }
    private final String host,expectedDeviceId;
    public RadioApi(String address){this(address, "");}
    public RadioApi(String address,String expectedId){host=RadioAddress.parse(address);expectedDeviceId=expectedId==null?"":expectedId;}
    private String request(String path,String body,State state)throws IOException {
        HttpURLConnection c=(HttpURLConnection)new URL("http://"+host+path).openConnection();
        try {
            c.setConnectTimeout(3000);c.setReadTimeout(4000);c.setInstanceFollowRedirects(false);
            c.setRequestProperty("Accept","application/json");c.setRequestProperty("Connection","close");
            c.setRequestProperty("Cache-Control","no-cache");
            if(body!=null){
                byte[] bytes=body.getBytes(StandardCharsets.UTF_8);
                c.setRequestMethod("POST");c.setDoOutput(true);c.setFixedLengthStreamingMode(bytes.length);
                c.setRequestProperty("Content-Type","text/plain; charset=utf-8");
                c.setRequestProperty("X-Radio-Token",state.token);c.setRequestProperty("X-Radio-Revision",state.revision);
                try(OutputStream out=c.getOutputStream()){out.write(bytes);}
            }
            int code=c.getResponseCode();
            InputStream stream=(code>=200&&code<300)?c.getInputStream():c.getErrorStream();
            ByteArrayOutputStream buffer=new ByteArrayOutputStream();
            if(stream!=null)try(InputStream in=stream){byte[] b=new byte[1024];int size;while((size=in.read(b))!=-1){if(buffer.size()+size>32768)throw new IOException("Radyo yanıtı boyut sınırını aştı.");buffer.write(b,0,size);}}
            String result=buffer.toString("UTF-8");
            if(code!=200){String message="Radyo HTTP "+code+" yanıtı verdi.";try{message=new JSONObject(result).optString("message",message);}catch(JSONException ignored){}throw new ApiError(code,message);}
            return result;
        }finally{c.disconnect();}
    }
    public State state()throws IOException {
        // A closed socket may race a new connection. Retry only the read, never an uncertain POST.
        for(int attempt=0;attempt<2;attempt++){
            try{State found=new State(new JSONObject(request("/api/state",null,null)));
                if(!expectedDeviceId.isEmpty()&&!expectedDeviceId.equals(found.deviceId))throw new ApiError(409,"Bu adreste başka bir cihaz var. Radyoları bul ile yeniden seçin.");
                return found;}
            catch(JSONException e){throw new IOException("Adres, beklenen radyo durumunu döndürmedi. IP ve firmware sürümünü kontrol edin.",e);}
            catch(ApiError e){throw e;}
            catch(IOException e){if(attempt==1)throw e;}
        }
        throw new IOException("Radyo durumu alınamadı.");
    }
    public State command(String command,String expectedRevision)throws IOException {
        // The server has one connection: the Activity serialises every operation.
        for(int attempt=0;attempt<2;attempt++) {
            State s=state();
            if(s.editing||!s.source.equals("radio"))throw new ApiError(409,"Cihazda Radyo kaynağına dönün ve açık ayar menüsünü kapatın.");
            if(command.startsWith("select:")&&!s.revision.equals(expectedRevision))throw new ApiError(409,"İstasyon listesi değişti; listeyi yenileyip tekrar seçin.");
            String body=command;
            if(command.equals("muteOn")){if(s.muted)return s;body="mute";}
            if(command.equals("unmute")){if(!s.muted)return s;body="mute";}
            try{request("/api/command",body,s);return state();}
            catch(ApiError e){if(e.code!=403||attempt!=0)throw e;}
        }
        throw new IOException("Komut tamamlanamadı.");
    }
}
