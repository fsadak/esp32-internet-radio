package tr.fsadak.radyokumanda;

public final class RadioAddress {
    private RadioAddress() {}
    public static String parse(String raw) {
        String s = raw.trim();
        if (s.startsWith("http://")) s = s.substring(7);
        if (s.endsWith("/")) s = s.substring(0,s.length()-1);
        if (s.endsWith(":80")) s = s.substring(0,s.length()-3);
        if (!s.matches("[0-9]{1,3}(\\.[0-9]{1,3}){3}")) throw new IllegalArgumentException("Yalnızca radyonun yerel IPv4 adresini yazın. Örnek: 192.168.1.50");
        String[] fields=s.split("\\."); int[] v=new int[4];
        for(int i=0;i<4;i++) {
            if(fields[i].length()>1 && fields[i].charAt(0)=='0') throw new IllegalArgumentException("IP adresinde baştaki sıfırları kaldırın.");
            v[i]=Integer.parseInt(fields[i]);
            if(v[i]>255) throw new IllegalArgumentException("Geçersiz IP adresi.");
        }
        boolean local=v[0]==10 || (v[0]==172 && v[1]>=16 && v[1]<=31) || (v[0]==192 && v[1]==168) || (v[0]==169 && v[1]==254);
        if(!local) throw new IllegalArgumentException("Radyonun yerel ağ adresini kullanın (192.168…, 10… veya 172.16–31…).");
        return s;
    }
}
