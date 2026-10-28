package tr.fsadak.radyokumanda;

import android.content.Context;
import android.net.nsd.*;
import android.os.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

/** Android DNS-SD browsing. All mutable state and callbacks are marshalled to main. */
public final class RadioDiscovery {
    public static final String TYPE="_esp32-radio._tcp.";
    public interface Listener {
        void onDevices(List<RadioDevices.Device> devices);
        void onStatus(String status);
        void onFinished();
    }
    private final NsdManager manager;
    private final Handler main=new Handler(Looper.getMainLooper());
    private NsdManager.DiscoveryListener discovery;
    private Listener listener;
    private RadioDevices devices=new RadioDevices();
    private final ArrayDeque<NsdServiceInfo> pending=new ArrayDeque<>();
    private final Set<String> seen=new HashSet<>(),lost=new HashSet<>();
    private boolean running,resolving;
    private int generation;
    public RadioDiscovery(Context context){manager=(NsdManager)context.getApplicationContext().getSystemService(Context.NSD_SERVICE);}
    private String key(NsdServiceInfo info){return info.getServiceName();}
    private void event(int gen,Runnable action){main.post(()->{if(running&&gen==generation)action.run();});}
    public void start(Listener callback){
        stop();listener=callback;devices=new RadioDevices();pending.clear();seen.clear();lost.clear();resolving=false;
        running=true;int gen=++generation;
        listener.onStatus("Aynı ağdaki radyolar aranıyor…");
        discovery=new NsdManager.DiscoveryListener(){
            public void onDiscoveryStarted(String type){}
            public void onServiceFound(NsdServiceInfo info){event(gen,()->{
                if(!info.getServiceType().startsWith("_esp32-radio._tcp")||seen.size()>=64)return;
                String k=key(info);lost.remove(k);
                if(seen.add(k)){pending.add(info);resolveNext(gen);}
            });}
            public void onServiceLost(NsdServiceInfo info){event(gen,()->{
                String k=key(info);lost.add(k);seen.remove(k);devices.removeService(k);listener.onDevices(devices.snapshot());
            });}
            public void onDiscoveryStopped(String type){}
            public void onStartDiscoveryFailed(String type,int error){event(gen,()->finish("Arama başlatılamadı. Wi-Fi bağlantısını kontrol edip tekrar deneyin."));}
            public void onStopDiscoveryFailed(String type,int error){}
        };
        try{manager.discoverServices(TYPE,NsdManager.PROTOCOL_DNS_SD,discovery);}
        catch(RuntimeException e){finish("Arama başlatılamadı. Telefonun yerel ağ erişimini kontrol edin.");return;}
        main.postDelayed(()->{if(running&&gen==generation)finish(devices.snapshot().isEmpty()?"Radyo bulunamadı. Firmware 0.11.0 veya sonrası yüklü olmalı; aynı Wi-Fi ağını kontrol edin.":"Arama tamamlandı. Bağlanmak için bir radyo seçin.");},15000);
    }
    @SuppressWarnings("deprecation")
    private void resolveNext(int gen){
        if(!running||gen!=generation||resolving)return;
        NsdServiceInfo next=pending.poll();if(next==null)return;
        if(lost.contains(key(next))){resolveNext(gen);return;}
        resolving=true;
        try{manager.resolveService(next,new NsdManager.ResolveListener(){
            public void onResolveFailed(NsdServiceInfo info,int error){event(gen,()->{resolving=false;resolveNext(gen);});}
            public void onServiceResolved(NsdServiceInfo info){event(gen,()->{
                resolving=false;
                if(!lost.contains(key(info)))addResolved(info);
                resolveNext(gen);
            });}
        });}catch(RuntimeException e){resolving=false;resolveNext(gen);}
    }
    private String attr(NsdServiceInfo info,String key){byte[] b=info.getAttributes().get(key);return b==null?"":new String(b,StandardCharsets.UTF_8);}
    @SuppressWarnings("deprecation")
    private void addResolved(NsdServiceInfo info){
        String id=attr(info,"id").toLowerCase(Locale.ROOT);
        if(info.getPort()!=80||!attr(info,"app").equals("esp32-internet-radio")||!id.matches("[a-f0-9]{12}"))return;
        List<InetAddress> addresses=new ArrayList<>();
        if(Build.VERSION.SDK_INT>=34)addresses.addAll(info.getHostAddresses());else if(info.getHost()!=null)addresses.add(info.getHost());
        for(InetAddress address:addresses){
            if(!(address instanceof Inet4Address))continue;
            try{
                String host=RadioAddress.parse(address.getHostAddress());
                devices.put(new RadioDevices.Device(id,info.getServiceName(),host,key(info)));
                listener.onDevices(devices.snapshot());return;
            }catch(IllegalArgumentException ignored){}
        }
    }
    private void finish(String status){Listener callback=listener;stop();if(callback!=null){callback.onStatus(status);callback.onFinished();}}
    public void stop(){
        running=false;generation++;listener=null;pending.clear();seen.clear();lost.clear();
        main.removeCallbacksAndMessages(null);
        if(discovery!=null){try{manager.stopServiceDiscovery(discovery);}catch(RuntimeException ignored){}discovery=null;}
    }
}
