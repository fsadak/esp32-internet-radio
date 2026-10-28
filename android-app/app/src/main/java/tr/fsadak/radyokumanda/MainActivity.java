package tr.fsadak.radyokumanda;

import android.Manifest;
import android.app.*;
import android.content.*;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.*;
import android.os.*;
import android.speech.*;
import android.view.*;
import android.view.inputmethod.InputMethodManager;
import android.widget.*;
import java.io.IOException;
import java.util.*;
import java.util.concurrent.*;

public class MainActivity extends Activity implements RecognitionListener {
    private static final int BG=0xff101114,CARD=0xff1b1d22,TEXT=0xfff5f3ed,DIM=0xffa5a6ae,ORANGE=0xffff8a38;
    private final Handler handler=new Handler(Looper.getMainLooper());
    private final ExecutorService worker=Executors.newSingleThreadExecutor();
    private final List<Button> controls=new ArrayList<>();
    private LinearLayout root,stationList;
    private EditText ip;
    private TextView connection,station,track,volume,details,feedback,stationCount,heardText;
    private Button connect,mic,mute,discoverButton;
    private RadioDiscovery discovery;
    private AlertDialog discoveryDialog;
    private LinearLayout foundList;
    private TextView discoveryStatus;
    private List<RadioDevices.Device> foundDevices=new ArrayList<>();
    private RadioDevices.Device pendingDevice;
    private boolean automaticSearch=false;
    private String selectedId="",connectedAddress="";
    private RadioApi api;
    private RadioApi.State state;
    private SpeechRecognizer recognizer;
    private boolean active=false,busy=false,listening=false,connected=false;
    private int generation=0;
    private String rowsKey="";
    private final Runnable poll=()->refresh(false);

    @Override public void onCreate(Bundle saved){super.onCreate(saved);buildUi();
        discovery=new RadioDiscovery(this);
        selectedId=getPreferences(MODE_PRIVATE).getString("deviceId","");
        ip.setText(getPreferences(MODE_PRIVATE).getString("ip",""));
        feedback.setText("Konuş düğmesine basarak komut verin.\nÖrnek: “Sesi azalt” veya “TRT FM aç”.");
        updateControls();
    }
    private int dp(float value){return Math.round(value*getResources().getDisplayMetrics().density);}
    private GradientDrawable shape(int color,int radius,int stroke){GradientDrawable d=new GradientDrawable();d.setColor(color);d.setCornerRadius(dp(radius));if(stroke!=0)d.setStroke(dp(1),stroke);return d;}
    private TextView text(String value,int size,int color){TextView t=new TextView(this);t.setText(value);t.setTextSize(size);t.setTextColor(color);t.setIncludeFontPadding(false);return t;}
    private Button button(String title,boolean accent){Button b=new Button(this);b.setText(title);b.setTextSize(16);b.setTextColor(accent?BG:TEXT);b.setAllCaps(false);b.setMinHeight(dp(50));b.setMinimumWidth(0);b.setPadding(dp(8),dp(4),dp(8),dp(4));
        StateListDrawable draw=new StateListDrawable();draw.addState(new int[]{android.R.attr.state_pressed},shape(0xffb86429,14,0));draw.addState(new int[]{},shape(accent?ORANGE:0xff292c33,14,0));b.setBackground(draw);return b;}
    private LinearLayout vertical(){LinearLayout l=new LinearLayout(this);l.setOrientation(LinearLayout.VERTICAL);return l;}
    private LinearLayout row(){LinearLayout l=new LinearLayout(this);l.setOrientation(LinearLayout.HORIZONTAL);l.setGravity(Gravity.CENTER_VERTICAL);return l;}
    private void add(LinearLayout p,View v,int height,int margin){LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(-1,height<0?height:dp(height));lp.topMargin=dp(margin);p.addView(v,lp);}
    private void share(LinearLayout p,View v,int weight){LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,dp(52),weight);if(p.getChildCount()>0)lp.leftMargin=dp(8);p.addView(v,lp);}
    private LinearLayout card(){LinearLayout l=vertical();l.setPadding(dp(16),dp(16),dp(16),dp(16));l.setBackground(shape(CARD,20,0));return l;}
    private void buildUi(){
        ScrollView scroll=new ScrollView(this);scroll.setFillViewport(true);scroll.setBackgroundColor(BG);
        root=vertical();root.setPadding(dp(20),dp(20),dp(20),dp(24));scroll.addView(root);setContentView(scroll);
        scroll.setOnApplyWindowInsetsListener((v,insets)->{
            int top,bottom,left,right;
            if(Build.VERSION.SDK_INT>=30){android.graphics.Insets b=insets.getInsets(WindowInsets.Type.systemBars()|WindowInsets.Type.displayCutout());top=b.top;bottom=b.bottom;left=b.left;right=b.right;}
            else{top=insets.getSystemWindowInsetTop();bottom=insets.getSystemWindowInsetBottom();left=insets.getSystemWindowInsetLeft();right=insets.getSystemWindowInsetRight();}
            v.setPadding(left,top,right,bottom);return insets;
        });scroll.requestApplyInsets();
        TextView eyebrow=text("ESP32  /  İNTERNET RADYOSU",12,ORANGE);eyebrow.setLetterSpacing(.1f);add(root,eyebrow,-2,0);
        TextView title=text("Radyo Kumandası",29,TEXT);title.setTypeface(null,Typeface.BOLD);add(root,title,-2,8);
        add(root,text("Telefonunuzdan seçin, konuşarak yönetin.",14,DIM),-2,6);

        LinearLayout network=card();add(root,network,-2,22);add(network,text("RADYO BAĞLANTISI",12,DIM),-2,0);
        discoverButton=button("Radyoları bul",true);add(network,discoverButton,52,12);
        discoverButton.setOnClickListener(v->startDiscovery(true));
        TextView manual=text("IP ile bağlan",13,DIM);manual.setMinHeight(dp(44));manual.setGravity(Gravity.CENTER_VERTICAL);add(network,manual,-2,2);
        LinearLayout address=row();add(network,address,-2,4);address.setVisibility(View.GONE);
        manual.setOnClickListener(v->address.setVisibility(address.getVisibility()==View.GONE?View.VISIBLE:View.GONE));
        ip=new EditText(this);ip.setSingleLine(true);ip.setTextSize(16);ip.setTextColor(TEXT);ip.setHintTextColor(DIM);ip.setHint("192.168.1.50");ip.setInputType(android.text.InputType.TYPE_CLASS_TEXT|android.text.InputType.TYPE_TEXT_VARIATION_URI);ip.setSelectAllOnFocus(true);ip.setContentDescription("Radyo IP adresi");
        address.addView(ip,new LinearLayout.LayoutParams(0,dp(52),1));connect=button("Bağlan",false);LinearLayout.LayoutParams clp=new LinearLayout.LayoutParams(dp(96),dp(50));clp.leftMargin=dp(8);address.addView(connect,clp);
        connection=text("Radyoları bul ile cihazınızı seçin.",13,DIM);add(network,connection,-2,10);
        connect.setOnClickListener(v->connect());ip.setOnEditorActionListener((v,action,event)->{connect();return true;});

        LinearLayout now=card();add(root,now,-2,14);add(now,text("ŞİMDİ ÇALIYOR",12,ORANGE),-2,0);
        station=text("Radyoya bağlanın",24,TEXT);station.setTypeface(null,Typeface.BOLD);add(now,station,-2,12);
        track=text("İstasyon ve parça bilgileri burada görünecek.",16,DIM);add(now,track,-2,8);
        details=text("",12,DIM);add(now,details,-2,8);
        LinearLayout nav=row();add(now,nav,-2,18);commandButton(nav,"‹ Önceki","previous");commandButton(nav,"Sonraki ›","next");
        LinearLayout sound=row();add(now,sound,-2,10);
        Button down=button("−",false);down.setTextSize(26);down.setContentDescription("Sesi azalt");sound.addView(down,new LinearLayout.LayoutParams(dp(64),dp(52)));down.setOnClickListener(v->send("volumeDown",null));controls.add(down);
        volume=text("Ses — / —",18,TEXT);volume.setGravity(Gravity.CENTER);sound.addView(volume,new LinearLayout.LayoutParams(0,dp(52),1));
        Button up=button("+",false);up.setTextSize(26);up.setContentDescription("Sesi artır");sound.addView(up,new LinearLayout.LayoutParams(dp(64),dp(52)));up.setOnClickListener(v->send("volumeUp",null));controls.add(up);
        mute=button("Sessize al",false);add(now,mute,50,10);mute.setOnClickListener(v->send(state!=null&&state.muted?"unmute":"muteOn",null));controls.add(mute);
        LinearLayout playback=row();add(now,playback,-2,10);commandButton(playback,"Başlat","play");commandButton(playback,"Durdur","stop");

        LinearLayout voice=card();add(root,voice,-2,14);add(voice,text("TÜRKÇE SESLİ KUMANDA",12,ORANGE),-2,0);
        mic=button("Konuş",true);add(voice,mic,56,12);mic.setOnClickListener(v->{if(listening){recognizer.stopListening();mic.setEnabled(false);}else startVoice();});
        heardText=text("",14,TEXT);add(voice,heardText,-2,12);
        feedback=text("",14,DIM);add(voice,feedback,-2,8);
        TextView examples=text("Komut örneklerini göster",13,ORANGE);examples.setPadding(0,dp(12),0,dp(8));examples.setMinHeight(dp(48));examples.setGravity(Gravity.CENTER_VERTICAL);add(voice,examples,-2,4);examples.setOnClickListener(v->new AlertDialog.Builder(this).setTitle("Söyleyebilecekleriniz").setMessage("• Sesi artır / Sesi azalt\n• Sessize al / Sesi geri aç\n• Sonraki istasyon / Önceki istasyon\n• TRT FM aç (listenizde varsa)\n• Radyo yedi aç (listenizde varsa)\n• Yayını başlat / Yayını durdur\n\nKomutlar yalnızca düğmeye bastığınızda dinlenir. Türkçe tanıma, telefonunuzdaki konuşma hizmetine göre internet kullanabilir. ‘Radyoyu kapat’ yayını durdurur; cihazın gücünü kesmez.").setPositiveButton("Tamam",null).show());

        stationCount=text("İSTASYONLAR",12,DIM);add(root,stationCount,-2,24);stationList=vertical();add(root,stationList,-2,6);
        add(root,text("v0.2.0 · Telefon ve radyo aynı yerel ağda olmalı.\nSD oynatma ve cihaz ayarları radyo ekranından yönetilir.",12,DIM),-2,20);
    }
    private void commandButton(LinearLayout row,String label,String command){Button b=button(label,false);share(row,b,1);controls.add(b);b.setOnClickListener(v->send(command,null));}
    private void connect(){
        try{if(!busy){closeDiscovery();connectAddress(RadioAddress.parse(ip.getText().toString()),"");}}
        catch(IllegalArgumentException e){connection.setText(e.getMessage());}
    }
    private void connectAddress(String address,String deviceId){
        handler.removeCallbacks(poll);generation++;connectedAddress=address;selectedId=deviceId;
        api=new RadioApi(address,deviceId);state=null;connected=false;rowsKey="";stationList.removeAllViews();
        getPreferences(MODE_PRIVATE).edit().putString("ip",address).putString("deviceId",deviceId).apply();
        ip.setText(address);ip.clearFocus();
        ((InputMethodManager)getSystemService(INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(ip.getWindowToken(),0);
        connection.setText("Bağlanılıyor…");refresh(true);
    }
    private String deviceLabel(RadioDevices.Device d){return getPreferences(MODE_PRIVATE).getString("alias_"+d.id,d.name);}
    private void showDevices(List<RadioDevices.Device> devices){
        foundDevices=new ArrayList<>(devices);
        if(foundList==null)return;
        foundList.removeAllViews();
        for(RadioDevices.Device d:devices){
            Button b=button(deviceLabel(d)+"\n"+d.address,false);b.setGravity(Gravity.START|Gravity.CENTER_VERTICAL);b.setPadding(dp(14),dp(6),dp(10),dp(6));
            add(foundList,b,68,8);
            b.setOnClickListener(v->{pendingDevice=d;closeDiscovery();tryPendingDevice();});
            b.setOnLongClickListener(v->{
                EditText name=new EditText(this);name.setSingleLine(true);name.setText(deviceLabel(d));name.setSelectAllOnFocus(true);
                new AlertDialog.Builder(this).setTitle("Radyoya ad verin").setView(name)
                    .setPositiveButton("Kaydet",(dialog,which)->{String value=name.getText().toString().trim();if(value.length()>40)value=value.substring(0,40);
                        if(!value.isEmpty())getPreferences(MODE_PRIVATE).edit().putString("alias_"+d.id,value).apply();
                        showDevices(foundDevices);if(state!=null)applyState(state);
                    }).setNegativeButton("Vazgeç",null).show();return true;
            });
        }
    }
    private boolean tryPendingDevice(){
        if(pendingDevice==null||busy||listening||!active)return false;
        RadioDevices.Device d=pendingDevice;pendingDevice=null;connectAddress(d.address,d.id);return true;
    }
    private void closeDiscovery(){
        automaticSearch=false;if(discovery!=null)discovery.stop();
        if(discoveryDialog!=null){AlertDialog dialog=discoveryDialog;discoveryDialog=null;dialog.dismiss();}
        foundList=null;discoveryStatus=null;
    }
    private void startDiscovery(boolean showDialog){
        if(discovery==null||!active||listening)return;
        closeDiscovery();automaticSearch=!showDialog;
        if(showDialog){
            handler.removeCallbacks(poll);foundDevices.clear();
            LinearLayout panel=vertical();panel.setPadding(dp(20),dp(8),dp(20),dp(8));
            discoveryStatus=text("Radyolar aranıyor…",14,DIM);add(panel,discoveryStatus,-2,0);
            ScrollView scroll=new ScrollView(this);foundList=vertical();scroll.addView(foundList);add(panel,scroll,280,8);
            add(panel,text("Bağlanmak için dokunun. Ad vermek için basılı tutun.",12,DIM),-2,10);
            discoveryDialog=new AlertDialog.Builder(this).setTitle("Ağdaki radyolar").setView(panel)
                .setPositiveButton("Tekrar ara",null).setNegativeButton("Kapat",null).create();
            discoveryDialog.setOnDismissListener(dialog->{if(discoveryDialog==dialog)discoveryDialog=null;discovery.stop();automaticSearch=false;foundList=null;discoveryStatus=null;schedulePoll();});
            discoveryDialog.setOnShowListener(dialog->discoveryDialog.getButton(AlertDialog.BUTTON_POSITIVE).setOnClickListener(v->startDiscovery(true)));
            discoveryDialog.show();
        }
        discovery.start(new RadioDiscovery.Listener(){
            public void onDevices(List<RadioDevices.Device> devices){
                if(!active)return;
                if(automaticSearch){
                    for(RadioDevices.Device d:devices)if(d.id.equals(selectedId)){
                        if(!connected||!d.address.equals(connectedAddress)){pendingDevice=d;closeDiscovery();tryPendingDevice();}else closeDiscovery();
                        return;
                    }
                }else{showDevices(devices);if(discoveryStatus!=null)discoveryStatus.setText(devices.isEmpty()?"Radyolar aranıyor…":devices.size()+" radyo bulundu. Arama sürüyor…");}
            }
            public void onStatus(String message){if(discoveryStatus!=null)discoveryStatus.setText(message);}
            public void onFinished(){automaticSearch=false;}
        });
    }
    private void updateControls(){boolean ready=connected&&state!=null&&!state.editing&&state.source.equals("radio")&&!busy&&!listening;
        for(Button b:controls){b.setEnabled(ready);b.setAlpha(ready?1f:.4f);}
        mic.setEnabled(ready||listening);mic.setAlpha(mic.isEnabled()?1f:.4f);connect.setEnabled(!busy&&!listening);discoverButton.setEnabled(!listening);
    }
    private void refresh(boolean requested){
        if(api==null||busy||!active||listening)return;
        busy=true;updateControls();int gen=generation;RadioApi target=api;
        worker.execute(()->{try{RadioApi.State next=target.state();handler.post(()->{if(gen!=generation||isDestroyed())return;busy=false;connected=true;applyState(next);schedulePoll();});}
            catch(Exception e){handler.post(()->{if(gen!=generation||isDestroyed())return;busy=false;connected=false;connection.setText(error(e));updateControls();schedulePoll();});}});
    }
    private String error(Exception e){if(e instanceof RadioApi.ApiError)return e.getMessage();if(e instanceof java.net.SocketTimeoutException)return "Radyo yanıt vermedi. Aynı Wi-Fi ağını ve IP adresini kontrol edin.";return "Bağlantı kurulamadı. Radyo açık mı, IP doğru mu?";}
    private void schedulePoll(){handler.removeCallbacks(poll);if(tryPendingDevice())return;if(active&&api!=null&&(discoveryDialog==null||!discoveryDialog.isShowing()))handler.postDelayed(poll,2500);}
    private void applyState(RadioApi.State s){
        state=s;
        if(!s.deviceId.isEmpty()&&!s.deviceId.equals(selectedId)){
            selectedId=s.deviceId;getPreferences(MODE_PRIVATE).edit().putString("deviceId",selectedId).apply();api=new RadioApi(connectedAddress,selectedId);
        }
        String label=s.deviceId.isEmpty()?"":getPreferences(MODE_PRIVATE).getString("alias_"+s.deviceId,s.deviceName);
        connection.setText(s.source.equals("radio")?(s.editing?"Bağlı · Cihazdaki ayar menüsünü kapatın.":"Bağlı · "+(label.isEmpty()?"":""+label+" · ")+"Firmware "+s.version):"Bağlı · Kumanda için cihazda Radyo kaynağına dönün.");
        station.setText(s.station.isEmpty()?"İstasyon seçin":s.station);track.setText(s.track.isEmpty()?(s.playing?"Yayın çalıyor":s.status):s.track);
        details.setText(s.status);volume.setText("Ses "+s.volume+" / "+s.maxVolume);mute.setText(s.muted?"Sesi geri aç":"Sessize al");
        String key=s.revision+"|"+s.current+"|"+s.stations.toString();
        if(!key.equals(rowsKey)){
            rowsKey=key;stationList.removeAllViews();stationCount.setText("İSTASYONLAR  ·  "+s.stations.size());
            // Old buttons are released rather than accumulating on every poll.
            controls.removeIf(b->b.getTag()!=null&&b.getTag().equals("station"));
            for(int i=0;i<s.stations.size();i++){
                final int index=i;final String revision=s.revision;Button b=button((i==s.current?"●  ":"")+s.stations.get(i),false);
                b.setGravity(Gravity.CENTER_VERTICAL|Gravity.START);b.setPadding(dp(16),0,dp(12),0);b.setTag("station");b.setTextColor(i==s.current?ORANGE:TEXT);
                add(stationList,b,52,7);controls.add(b);b.setOnClickListener(v->send("select:"+index,revision));
            }
        }updateControls();
    }
    private void send(String command,String revision){
        if(api==null||busy||!connected)return;
        busy=true;handler.removeCallbacks(poll);updateControls();int gen=generation;RadioApi target=api;
        worker.execute(()->{try{RadioApi.State next=target.command(command,revision);handler.post(()->{if(gen!=generation||isDestroyed())return;busy=false;connected=true;applyState(next);feedback.setText(command.equals("stop")?"Yayın durduruldu.":"Komut uygulandı.");schedulePoll();});}
            catch(Exception e){handler.post(()->{if(gen!=generation||isDestroyed())return;busy=false;if(!(e instanceof RadioApi.ApiError))connected=false;feedback.setText(error(e));updateControls();schedulePoll();});}});
    }
    private void startVoice(){
        if(checkSelfPermission(Manifest.permission.RECORD_AUDIO)!=PackageManager.PERMISSION_GRANTED){requestPermissions(new String[]{Manifest.permission.RECORD_AUDIO},10);return;}
        if(!SpeechRecognizer.isRecognitionAvailable(this)){feedback.setText("Telefonda konuşma tanıma hizmeti bulunamadı. Sesle yazma hizmetini etkinleştirin.");return;}
        if(recognizer==null){recognizer=SpeechRecognizer.createSpeechRecognizer(this);recognizer.setRecognitionListener(this);}
        Intent intent=new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE,"tr-TR");intent.putExtra(RecognizerIntent.EXTRA_MAX_RESULTS,5);intent.putExtra(RecognizerIntent.EXTRA_PARTIAL_RESULTS,false);
        try{handler.removeCallbacks(poll);listening=true;mic.setText("Dinlemeyi bitir");heardText.setText("");feedback.setText("Dinliyorum… Komutunuzu söyleyin.");updateControls();recognizer.startListening(intent);}
        catch(RuntimeException e){finishVoice();feedback.setText("Konuşma hizmeti başlatılamadı. Telefonun sesle yazma ayarlarını kontrol edin.");}
    }
    private void finishVoice(){listening=false;mic.setText("Konuş");updateControls();schedulePoll();}
    @Override public void onResults(Bundle bundle){finishVoice();if(!active)return;ArrayList<String> results=bundle.getStringArrayList(SpeechRecognizer.RESULTS_RECOGNITION);
        if(results==null||results.isEmpty()||state==null){feedback.setText("Konuşma algılanamadı. Tekrar deneyin.");return;}
        // Execute only the top transcription, never a lower-confidence alternative.
        String heard=results.get(0);VoiceRules.Result result=VoiceRules.parse(heard,state.stations);
        heardText.setText("Duyulan: “"+heard+"”");feedback.setText(result.message);
        if(result.command!=null)send(result.command,state.revision);
    }
    @Override public void onError(int code){finishVoice();String reason;
        switch(code){case SpeechRecognizer.ERROR_NO_MATCH:case SpeechRecognizer.ERROR_SPEECH_TIMEOUT:reason="Komut duyulamadı. Tekrar deneyin.";break;case SpeechRecognizer.ERROR_INSUFFICIENT_PERMISSIONS:reason="Sesli komut için mikrofon izni gerekiyor.";break;case SpeechRecognizer.ERROR_NETWORK:case SpeechRecognizer.ERROR_NETWORK_TIMEOUT:reason="Konuşma hizmetine erişilemiyor. Telefonun internetini kontrol edin.";break;default:reason="Konuşma tanıma tamamlanamadı ("+code+"). Tekrar deneyin.";}
        feedback.setText(reason);
    }
    @Override public void onRequestPermissionsResult(int request,String[] permissions,int[] grants){super.onRequestPermissionsResult(request,permissions,grants);if(request==10){if(grants.length>0&&grants[0]==PackageManager.PERMISSION_GRANTED)startVoice();else feedback.setText("Mikrofon izni verilmedi. Düğmelerle kumanda etmeye devam edebilirsiniz.");}}
    @Override protected void onResume(){super.onResume();active=true;
        if(api==null&&!ip.getText().toString().isEmpty())try{connectedAddress=RadioAddress.parse(ip.getText().toString());api=new RadioApi(connectedAddress,selectedId);}catch(IllegalArgumentException ignored){}
        refresh(false);
        if(api==null)startDiscovery(true);else if(!selectedId.isEmpty())startDiscovery(false);
    }
    @Override protected void onPause(){active=false;pendingDevice=null;closeDiscovery();handler.removeCallbacks(poll);if(recognizer!=null){recognizer.cancel();finishVoice();}super.onPause();}
    @Override protected void onDestroy(){closeDiscovery();handler.removeCallbacksAndMessages(null);if(recognizer!=null)recognizer.destroy();worker.shutdownNow();super.onDestroy();}
    @Override public void onReadyForSpeech(Bundle b){}@Override public void onBeginningOfSpeech(){}@Override public void onRmsChanged(float rms){}@Override public void onBufferReceived(byte[] b){}@Override public void onEndOfSpeech(){}@Override public void onPartialResults(Bundle b){}@Override public void onEvent(int type,Bundle b){}
}
