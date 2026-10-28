package tr.fsadak.radyokumanda;
import java.util.*;

/** Identity, not IP, is the key: DHCP address changes must not duplicate a radio. */
public final class RadioDevices {
    public static final class Device {
        public final String id,name,address,service;
        public Device(String id,String name,String address,String service){this.id=id;this.name=name;this.address=address;this.service=service;}
    }
    private final Map<String,Device> devices=new LinkedHashMap<>();
    public void put(Device device){if(devices.size()<32||devices.containsKey(device.id))devices.put(device.id,device);}
    public void removeService(String service){devices.values().removeIf(d->d.service.equals(service));}
    public List<Device> snapshot(){List<Device> out=new ArrayList<>(devices.values());out.sort(Comparator.comparing(d->d.name));return out;}
}
