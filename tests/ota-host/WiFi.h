#pragma once
#include "Arduino.h"
#include <memory>
#include <cstring>
struct FakeConnection {std::string input,output;size_t readAt=0;bool valid=true,connected=true;};
extern std::shared_ptr<FakeConnection> incoming;
class WiFiClient {
    std::shared_ptr<FakeConnection> connection;
public:
    WiFiClient(std::shared_ptr<FakeConnection> c=nullptr):connection(c){}
    int fd()const{return connection&&connection->valid?4:-1;}
    void stop(){if(connection)connection->valid=false;}
    int available()const{return fd()<0?0:connection->input.size()-connection->readAt;}
    int read(){return available()?static_cast<unsigned char>(connection->input[connection->readAt++]):-1;}
    int read(uint8_t* out,size_t size){size=std::min(size,static_cast<size_t>(available()));memcpy(out,connection->input.data()+connection->readAt,size);connection->readAt+=size;return size;}
    bool connected()const{return fd()>=0&&connection->connected;}
};
class WiFiServer {
public:
    WiFiServer(int,int){}
    void begin(){} void setNoDelay(bool){}
    WiFiClient available(){auto c=incoming;incoming.reset();return WiFiClient(c);}
};
struct FakeIP{String toString()const{return "192.168.1.10";}};
struct FakeWiFi{FakeIP localIP()const{return {};}};
inline FakeWiFi WiFi;
