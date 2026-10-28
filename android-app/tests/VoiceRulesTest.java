import tr.fsadak.radyokumanda.*;
import java.util.*;
public class VoiceRulesTest {
    static int count;
    static List<String> stations=Arrays.asList("TRT FM","Radyo 7","Süper FM");
    static void check(String phrase,String expected){String actual=VoiceRules.parse(phrase,stations).command;if(!Objects.equals(actual,expected))throw new AssertionError(phrase+" => "+actual+", expected "+expected);count++;}
    public static void main(String[] args){
        check("Sesi artır","volumeUp");check("SESİ AÇ","volumeUp");check("Sesi yükselt","volumeUp");check("Sesi biraz aç","volumeUp");
        check("Sesi kıs","volumeDown");check("Sesi azalt","volumeDown");check("Sesi düşür","volumeDown");
        check("Sessize al","muteOn");check("Sesi kapat","muteOn");check("Sesi geri aç","unmute");check("Sessizden çıkar","unmute");
        check("Sonraki istasyon","next");check("Önceki kanal","previous");
        check("Radyoyu aç","play");check("Radyoyu kapat","stop");check("Yayını durdur","stop");
        check("TRT FM aç","select:0");check("TRT FM'i aç","select:0");check("TRT FM istasyonunu aç","select:0");
        check("te re te fm aç","select:0");check("Radyo yedi aç","select:1");check("Radyo yediyi aç","select:1");check("Süper FM'i aç","select:2");
        check("Bütün istasyonları sil",null);check("Sesi artır sonra kapat",null);check("Spotify aç",null);check("",null);check("FM aç",null);
        if(VoiceRules.parse("TRT FM aç",Arrays.asList("TRT FM","TRT Fm")).command!=null)throw new AssertionError("Ambiguous station must not be selected");count++;
        for(String s:Arrays.asList("192.168.1.50","10.0.2.2","172.16.1.1","http://192.168.1.50:80/")){RadioAddress.parse(s);count++;}
        for(String s:Arrays.asList("example.com","8.8.8.8","192.168.1.999","192.168.1.2/api/state","192.168.1.2:8080","https://192.168.1.2","http://user@192.168.1.2","192.168.01.2","172.32.0.1")){
            boolean rejected=false;try{RadioAddress.parse(s);}catch(IllegalArgumentException e){rejected=true;}if(!rejected)throw new AssertionError("Invalid radio address accepted: "+s);count++;
        }
        System.out.println("PASS: "+count+" voice/address cases");
    }
}
