#pragma once
#include <string>
#include <stdint.h>
#include <stddef.h>
class String {
    std::string value_;
public:
    String(const char* value = "") : value_(value) {}
    String& operator=(const char* value) { value_ = value; return *this; }
    const char* c_str() const { return value_.c_str(); }
};
