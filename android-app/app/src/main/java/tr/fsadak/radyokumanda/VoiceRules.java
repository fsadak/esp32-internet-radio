package tr.fsadak.radyokumanda;

import java.text.Normalizer;
import java.util.*;

/** Deliberately bounded grammar. Unrecognised or ambiguous speech never mutates the radio. */
public final class VoiceRules {
    public static final class Result {
        public final String command, message;
        public Result(String c,String m){command=c;message=m;}
    }
    public static String normal(String value) {
        String s=value.toLowerCase(Locale.forLanguageTag("tr-TR")).replace('ı','i');
        s=Normalizer.normalize(s,Normalizer.Form.NFD).replaceAll("\\p{M}+","");
        s=s.replaceAll("[^a-z0-9 ]"," ").replaceAll("\\s+"," ").trim();
        s=s.replace("te re te","trt").replace("ti ar ti","trt");
        String[] words={"sifir","bir","iki","uc","dort","bes","alti","yedi","sekiz","dokuz"};
        for(int i=0;i<words.length;i++) s=s.replaceAll("\\b"+words[i]+"(yi|yu|i|u)?\\b",Integer.toString(i)+"$1");
        return s;
    }
    private static boolean is(String s,String... list){for(String v:list)if(s.equals(v))return true;return false;}
    public static Result parse(String speech,List<String> stations) {
        String s=normal(speech);
        if(is(s,"sesi geri ac","sessizden cikar","sessizi kapat"))return new Result("unmute","Ses geri açılıyor");
        if(is(s,"sessize al","sesi kapat","sessiz","radyoyu sessize al"))return new Result("muteOn","Sessize alınıyor");
        if(is(s,"sesi ac","ses ac","sesi artir","sesi yukselt","sesi biraz ac"))return new Result("volumeUp","Ses artırılıyor");
        if(is(s,"sesi azalt","ses azalt","sesi kis","ses kis","sesi dusur","sesi biraz kis"))return new Result("volumeDown","Ses azaltılıyor");
        if(is(s,"sonraki","sonraki istasyon","sonraki kanal","kanali degistir","istasyonu degistir"))return new Result("next","Sonraki istasyon");
        if(is(s,"onceki","onceki istasyon","onceki kanal","geri git"))return new Result("previous","Önceki istasyon");
        if(is(s,"radyoyu ac","radyo ac","oynat","cal","yayini baslat","radyoyu baslat"))return new Result("play","Yayın başlatılıyor");
        if(is(s,"radyoyu kapat","radyo kapat","durdur","yayini durdur","radyoyu durdur"))return new Result("stop","Yayın durduruluyor");
        String name=s.replaceFirst("^(lutfen |bana )","").replaceFirst(" (istasyonunu|kanalini|radyosunu)? ?(ac|cal|dinlet|oynat)$","").trim();
        ArrayList<Integer> matches=new ArrayList<>();
        for(int i=0;i<stations.size();i++) {
            String candidate=normal(stations.get(i));
            String compact=candidate.replace(" ",""),spoken=name.replace(" ","");
            if(!candidate.isEmpty() && (compact.equals(spoken)||spoken.equals(compact+"i")||spoken.equals(compact+"yi")||spoken.equals(compact+"u")||spoken.equals(compact+"yu")))matches.add(i);
        }
        if(matches.size()==1){int index=matches.get(0);return new Result("select:"+index,stations.get(index)+" açılıyor");}
        if(matches.size()>1)return new Result(null,"Birden fazla istasyon eşleşti. Listeden seçin.");
        return new Result(null,"Komut anlaşılamadı. Örnek: “Sesi azalt”, “Sonraki istasyon” veya listedeki istasyonun adı + “aç”.");
    }
}
