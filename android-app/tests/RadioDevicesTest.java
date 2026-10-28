import tr.fsadak.radyokumanda.*;
import java.util.*;
public class RadioDevicesTest {
 static int n;static void ok(boolean value){if(!value)throw new AssertionError("Discovery case "+n);n++;}
 public static void main(String[] args){
  RadioDevices d=new RadioDevices();
  d.put(new RadioDevices.Device("001122334455","Salon","192.168.1.20","serviceA"));
  d.put(new RadioDevices.Device("aabbccddeeff","Mutfak","192.168.1.21","serviceB"));
  ok(d.snapshot().size()==2);ok(d.snapshot().get(0).name.equals("Mutfak"));
  d.put(new RadioDevices.Device("001122334455","Salon","192.168.1.99","serviceA-new"));
  ok(d.snapshot().size()==2);ok(d.snapshot().get(1).address.equals("192.168.1.99"));
  d.removeService("serviceA");ok(d.snapshot().size()==2);
  d.removeService("serviceA-new");ok(d.snapshot().size()==1);
  d.removeService("unknown");ok(d.snapshot().size()==1);
  d.snapshot().clear();ok(d.snapshot().size()==1);
  for(int i=0;i<40;i++)d.put(new RadioDevices.Device("device"+i,"Radio"+i,"10.0.0.1","key"+i));
  ok(d.snapshot().size()==32);
  d.put(new RadioDevices.Device("aabbccddeeff","Mutfak","192.168.1.33","serviceB"));
  ok(d.snapshot().stream().anyMatch(x->x.id.equals("aabbccddeeff")&&x.address.endsWith("33")));
  System.out.println("PASS: "+n+" device identity/list cases");
 }
}
