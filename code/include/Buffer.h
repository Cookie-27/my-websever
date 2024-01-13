#pragma once
#include <memory>
#include <string>
#include "common.h"


class Buffer
{

public:
    Buffer() = default;
    ~Buffer() = default;
    //non copy and move
public:
    const std::string &buf() const;
    const char* c_str() const;
    void set_buf(const char *buf);

    size_t Size() const;
    void Append(const char *_str, int _size);
    void Clear();
    const char* findCRLF() const;
    void retrieveUntil(const char* end);

private:
    std::string buffer_; 
    static const char CRLF[2];   
    
};

