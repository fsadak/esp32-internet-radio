package tr.fsadak.radyokumanda;
import android.app.Instrumentation;
import android.content.Context;
import android.net.nsd.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.*;
import android.app.Activity;
import android.content.Intent;
import android.view.*;
import android.widget.*;

public final class DiscoveryChecks {
 public static String run(Instrumentation test)throws Exception {
  NsdManager nsd=(NsdManager)test.getTargetContext().getSystemService(Context.NSD_SERVICE);
  String[] ids={"001122334455","aabbccddeeff"};
  List<NsdManager.RegistrationListener> listeners=new ArrayList<>();
  CountDownLatch registered=new CountDownLatch(2),found=new CountDownLatch(1);
  AtomicReference<String> error=new AtomicReference<>();
  Set<String> observed=Collections.synchronizedSet(new HashSet<>());
  RadioDiscovery[] browser=new RadioDiscovery[1];
  try {
   for(int i=0;i<2;i++){
    NsdServiceInfo info=new NsdServiceInfo();info.setServiceName("Radio test "+i+" "+System.nanoTime());info.setServiceType(RadioDiscovery.TYPE);info.setPort(80);
    info.setAttribute("app","esp32-internet-radio");info.setAttribute("id",ids[i]);
    NsdManager.RegistrationListener listener=new NsdManager.RegistrationListener(){
     public void onServiceRegistered(NsdServiceInfo s){registered.countDown();}
     public void onRegistrationFailed(NsdServiceInfo s,int code){error.set("Registration "+code);registered.countDown();}
     public void onServiceUnregistered(NsdServiceInfo s){}
     public void onUnregistrationFailed(NsdServiceInfo s,int code){}
    };listeners.add(listener);nsd.registerService(info,NsdManager.PROTOCOL_DNS_SD,listener);
   }
   if(!registered.await(12,TimeUnit.SECONDS)||error.get()!=null)throw new AssertionError("NSD registration: "+error.get());
   test.runOnMainSync(()->{browser[0]=new RadioDiscovery(test.getTargetContext());browser[0].start(new RadioDiscovery.Listener(){
    public void onDevices(List<RadioDevices.Device> devices){for(RadioDevices.Device d:devices)observed.add(d.id);if(observed.containsAll(Arrays.asList(ids)))found.countDown();}
    public void onStatus(String value){}
    public void onFinished(){}
   });});
   if(!found.await(18,TimeUnit.SECONDS))throw new AssertionError("Two advertised radios were not found: "+observed);
   Intent launch=new Intent(test.getTargetContext(),MainActivity.class);launch.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
   Activity activity=test.startActivitySync(launch);test.waitForIdleSync();
   // On a clean install onResume opens the real discovery dialog automatically.
   java.lang.reflect.Field dialogField=MainActivity.class.getDeclaredField("discoveryDialog");dialogField.setAccessible(true);
   AtomicBoolean listed=new AtomicBoolean();
   for(int attempt=0;attempt<100&&!listed.get();attempt++){
    test.runOnMainSync(()->{try{
     android.app.AlertDialog dialog=(android.app.AlertDialog)dialogField.get(activity);
     if(dialog!=null)listed.set(countRadios(dialog.getWindow().getDecorView())>=2);
    }catch(IllegalAccessException e){throw new RuntimeException(e);}});
    if(!listed.get())Thread.sleep(100);
   }
   if(!listed.get())throw new AssertionError("Two discovered radios missing from selection dialog");
   android.graphics.Bitmap screenshot=test.getUiAutomation().takeScreenshot();
   try(java.io.FileOutputStream out=new java.io.FileOutputStream(new java.io.File(test.getTargetContext().getFilesDir(),"discovery-ui.png"))){screenshot.compress(android.graphics.Bitmap.CompressFormat.PNG,100,out);}
   return "PASS: two Android DNS-SD registrations discovered, resolved by identity, and listed in the selection dialog\n";
  }finally{
   test.runOnMainSync(()->{if(browser[0]!=null)browser[0].stop();});
   for(NsdManager.RegistrationListener l:listeners)try{nsd.unregisterService(l);}catch(RuntimeException ignored){}
  }
 }
 private static int countRadios(View view){
  int count=view instanceof Button&&((Button)view).getText().toString().startsWith("Radio test ")?1:0;
  if(view instanceof ViewGroup){ViewGroup group=(ViewGroup)view;for(int i=0;i<group.getChildCount();i++)count+=countRadios(group.getChildAt(i));}
  return count;
 }
}
