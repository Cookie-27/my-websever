#include "Buffer.h"
#include <algorithm>
#include <cstring>

const char Buffer::CRLF[2] = {'\r', '\n'};

const std::string &Buffer::buf() const
{
    return this->buffer_;
}

const char* Buffer::c_str() const
{
    return this->buffer_.c_str();
}
    
void Buffer::set_buf(const char *buf)
{
    std::string new_buffer(buf);
    this->buffer_ = new_buffer;
}

size_t Buffer::Size() const
{
    return buffer_.size();
}

void Buffer::Append(const char *_str, int _size)
{
    if (_str != nullptr && _size >= 0) {
        buffer_.append(_str, static_cast<size_t>(_size));
    }
}

void Buffer::Clear()
{
    buffer_.clear();
}

const char* Buffer::findCRLF() const
{
    const char* crlf = std::strstr(buffer_.c_str(), CRLF);
    return crlf;
}

void Buffer::retrieveUntil(const char* end)
{
    buffer_.erase(buffer_.begin(), buffer_.begin() + (end - buffer_.c_str()));
}
