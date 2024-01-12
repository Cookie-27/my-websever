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

    
private:
    std::string buffer_;    
};

