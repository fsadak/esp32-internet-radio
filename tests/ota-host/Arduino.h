#pragma once
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstddef>
extern uint32_t testMillis;
inline uint32_t millis(){return testMillis;}
class String {
    std::string s;
public:
    String(const char* value=""):s(value){}
    String& operator=(const char* value){s=value;return *this;}
    const char* c_str()const{return s.c_str();}
};
