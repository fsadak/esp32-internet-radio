#include "RadioOta.h"
#include "RadioWeb.h"
#include <iostream>
#include <cstdlib>
#include <vector>

uint32_t testMillis=1;
std::shared_ptr<FakeConnection> incoming,current;
esp_partition_t running{6553600},next{6553600};
bool noPartition=false,writeError=false,endError=false,activateError=false;
int begins=0,aborts=0,ends=0,activations=0,checks=0;
size_t maxWrite=0;std::string flash;
int rollbacks=0,confirmed=0;
esp_err_t esp_ota_get_state_partition(const esp_partition_t*,esp_ota_img_states_t* state){*state=ESP_OTA_IMG_PENDING_VERIFY;return ESP_OK;}
esp_err_t esp_ota_mark_app_invalid_rollback_and_reboot(){++rollbacks;return ESP_OK;}
esp_err_t esp_ota_mark_app_valid_cancel_rollback(){++confirmed;return ESP_OK;}
int send(int,const void* bytes,size_t size,int){current->output.append(static_cast<const char*>(bytes),size);return size;}
const esp_partition_t* esp_ota_get_next_update_partition(const void*){return noPartition?nullptr:&next;}
const esp_partition_t* esp_ota_get_running_partition(){return &running;}
esp_err_t esp_ota_begin(const esp_partition_t*,size_t mode,esp_ota_handle_t* h){if(mode!=OTA_WITH_SEQUENTIAL_WRITES)std::abort();++begins;flash.clear();*h=1;return ESP_OK;}
esp_err_t esp_ota_write(esp_ota_handle_t,const void* bytes,size_t size){maxWrite=std::max(maxWrite,size);flash.append(static_cast<const char*>(bytes),size);return writeError?1:ESP_OK;}
esp_err_t esp_ota_end(esp_ota_handle_t){++ends;return endError?1:ESP_OK;}
esp_err_t esp_ota_abort(esp_ota_handle_t){++aborts;return ESP_OK;}
esp_err_t esp_ota_set_boot_partition(const esp_partition_t*){++activations;return activateError?1:ESP_OK;}
void check(bool value,const char* label){++checks;if(!value){std::cerr<<"FAIL: "<<label<<'\n';std::exit(1);}}
std::string image(size_t size=12000){std::string data(size,'a');data[0]=char(0xe9);data[12]=9;data[13]=0;data[23]=1;data[32]=0x32;data[33]=0x54;data[34]=char(0xcd);data[35]=char(0xab);data.replace(4090,strlen(RADIO_FIRMWARE_ID),RADIO_FIRMWARE_ID);return data;}
bool blocked=false;RadioOta* ota=nullptr;int uploadBegin=0,uploadAbort=0;
void requestHandler(RadioWeb& web){web.reply(200,"text/plain",web.body);}
bool uploadHandler(RadioWeb& web,RadioWeb::Upload phase,const uint8_t* data,size_t size){
 if(phase==RadioWeb::Upload::Abort){++uploadAbort;ota->abort();return false;}
 if(phase==RadioWeb::Upload::Begin){++uploadBegin;if(blocked){web.reply(409,"text/plain","blocked");return false;}if(ota->begin(web.contentLength()))return true;}
 else if(phase==RadioWeb::Upload::Data){if(ota->write(data,size))return true;}
 else if(ota->finish()){web.reply(200,"text/plain","updated");return true;}
 web.reply(400,"text/plain",ota->error());return false;
}
std::string headers(RadioWeb& web,size_t size,std::string extra="",bool token=true){
 return "POST /api/firmware HTTP/1.1\r\nHost: 192.168.1.10\r\nOrigin: http://192.168.1.10\r\nContent-Type: application/octet-stream\r\nX-Radio-Confirm: update\r\nX-Radio-Token: "+std::string(token?web.token().c_str():"bad")+"\r\nContent-Length: "+std::to_string(size)+"\r\n"+extra+"\r\n";
}
void connect(const std::string& request){current=std::make_shared<FakeConnection>();current->input=request;incoming=current;}
void pump(RadioWeb& web,int count=500){for(int i=0;i<count&&current->valid;++i){++testMillis;web.loop();}}
int main(){
 RadioOta boot;boot.checkBoot(false);check(rollbacks==1&&confirmed==0,"failed startup rollback");testMillis=30000;boot.checkBoot(true);check(confirmed==0,"slow setup does not consume grace");testMillis+=9999;boot.checkBoot(true);check(confirmed==0,"full healthy interval required");testMillis+=1;boot.checkBoot(true);check(confirmed==1,"healthy boot confirmed after grace");boot.checkBoot(false);check(rollbacks==1,"confirmed boot not rolled back");
 const auto valid=image();
 for(size_t chunk:{size_t(1),size_t(17),size_t(4096)}){RadioFirmwareImage gate;gate.reset();for(size_t p=0;p<valid.size();p+=chunk)check(gate.feed(reinterpret_cast<const uint8_t*>(valid.data()+p),std::min(chunk,valid.size()-p)),"split header/marker");check(gate.compatible(),"valid product image");}
 for(size_t offset:{size_t(0),size_t(12),size_t(13),size_t(23),size_t(32)}){auto bad=valid;bad[offset]^=1;RadioFirmwareImage gate;check(!gate.feed(reinterpret_cast<const uint8_t*>(bad.data()),bad.size()),"invalid header rejected");}
 {RadioOta update;check(!update.begin(0),"zero rejected");check(!update.begin(7000000),"oversized rejected");noPartition=true;check(!update.begin(valid.size()),"missing slot rejected");noPartition=false;}
 {RadioOta update;check(update.begin(valid.size()),"begin valid");check(update.write(reinterpret_cast<const uint8_t*>(valid.data()),100),"partial write");int before=activations;check(!update.finish()&&activations==before&&!update.active(),"partial update not activated");}
 {RadioOta update;auto bad=valid;bad.replace(4090,strlen(RADIO_FIRMWARE_ID),strlen(RADIO_FIRMWARE_ID),'x');check(update.begin(bad.size())&&update.write(reinterpret_cast<const uint8_t*>(bad.data()),bad.size()),"foreign body received");int before=activations;check(!update.finish()&&activations==before,"foreign product not activated");}
 {RadioOta update;check(update.begin(valid.size()),"begin verify failure");check(update.write(reinterpret_cast<const uint8_t*>(valid.data()),valid.size()),"valid write");endError=true;int before=activations;check(!update.finish()&&activations==before,"invalid checksum not activated");endError=false;}
 {RadioOta update;update.begin(valid.size());writeError=true;check(!update.write(reinterpret_cast<const uint8_t*>(valid.data()),valid.size())&&!update.active(),"flash failure aborts");writeError=false;}
 RadioWeb web;web.begin(requestHandler,uploadHandler);
 for(const auto& replacement:std::vector<std::pair<std::string,std::string>>{{"Host: 192.168.1.10","Host: evil.example"},{"Origin: http://192.168.1.10","Origin: http://evil.example"},{"X-Radio-Confirm: update","X-Radio-Confirm: no"},{"Content-Type: application/octet-stream","Content-Type: multipart/form-data"}}){RadioOta update;ota=&update;int before=uploadBegin;auto request=headers(web,valid.size());request.replace(request.find(replacement.first),replacement.first.size(),replacement.second);connect(request+valid);pump(web);check(uploadBegin==before&&current->output.find("200 OK")==std::string::npos,"host/origin/confirm/type gated before flash");}
 {RadioOta update;ota=&update;int before=uploadBegin;connect(headers(web,valid.size(),"",false)+valid);pump(web);check(current->output.find("403")!=std::string::npos&&uploadBegin==before,"token before flash");}
 {RadioOta update;ota=&update;int before=uploadBegin;connect(headers(web,valid.size(),"Transfer-Encoding: chunked\r\n")+valid);pump(web);check(current->output.find("400")!=std::string::npos&&uploadBegin==before,"chunked rejected before flash");}
 {RadioOta update;ota=&update;int before=uploadBegin;connect(headers(web,valid.size(),"Content-Length: 1\r\n")+valid);pump(web);check(current->output.find("400")!=std::string::npos&&uploadBegin==before,"duplicate length rejected");}
 {RadioOta update;ota=&update;blocked=true;connect(headers(web,valid.size())+valid);pump(web);check(current->output.find("409")!=std::string::npos&&!update.active(),"settings lock");blocked=false;}
 {RadioOta update;ota=&update;connect(headers(web,valid.size())+valid.substr(0,5000));pump(web,5);check(update.active(),"upload streaming");int before=aborts;current->connected=false;pump(web);check(!update.active()&&aborts>before&&!update.pendingRestart(),"disconnect abort");}
 {RadioOta update;ota=&update;connect(headers(web,valid.size()));pump(web,2);check(update.active(),"header starts upload");testMillis+=15001;pump(web);check(!update.active()&&!update.pendingRestart(),"idle timeout abort");}
 {RadioOta update;ota=&update;maxWrite=0;connect(headers(web,valid.size())+valid);pump(web);check(current->output.find("200 OK")!=std::string::npos&&flash==valid&&maxWrite<=4096,"bounded streaming bytes intact");check(update.pendingRestart()&&!update.restartReady(),"response before restart");testMillis+=2000;check(update.restartReady(),"restart delayed");}
 connect("POST /api/command HTTP/1.1\r\nHost: 192.168.1.10\r\nX-Radio-Token: "+std::string(web.token().c_str())+"\r\nContent-Length: 4\r\n\r\nplay");pump(web);check(current->output.find("\r\n\r\nplay")!=std::string::npos,"normal API preserved");
 std::cout<<"PASS: "<<checks<<" OTA image, flash lifecycle, and HTTP transport checks\n";
}
