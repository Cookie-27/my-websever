#include "Socket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "common.h"

Socket::Socket():fd_(-1)
{
}

Socket::~Socket()
{
    if(fd_ != -1){
        close(fd_);
        fd_ = -1;
    }
}

void Socket::set_fd(int fd)
{
    this->fd_ = fd;
}

int Socket::fd() const
{
    return fd_;
}

std::string Socket::get_addr() const
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    memset(&addr, 0, addr_len);

    if (getpeername(fd_, reinterpret_cast<struct sockaddr*>(&addr), &addr_len) == -1) {
        //return "Unknown Address";
    }

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN) == nullptr) {
        //return "Unknown Address";
    }

    int port = ntohs(addr.sin_port);
    return std::string(ip_str) + ":" + std::to_string(port);
}

RS Socket::Create()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1) {
        perror("Failed to create socket");
        return RS::RS_DEFEAT;
    }
    return RS::RS_SUCCES;
}

RS Socket::Bind(const char *ip, uint16_t port) const
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (ip != nullptr) {
        if (inet_pton(AF_INET, ip, &(addr.sin_addr)) != 1) {
            // IP 地址转换失败，可以根据实际情况处理错误
            //throw std::runtime_error("Failed to convert IP address");
        }
    }
    if (bind(fd_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
        //throw std::runtime_error("Failed to bind the socket");
    }

    return RS::RS_SUCCES; 
}

RS Socket::Listen() const
{

}

RS Socket::Accept(int &clnt_fd) const
{
    
}

RS Socket::Connect(const char *ip, uint16_t port) const
{

}

RS Socket::SetBlock() const
{

}

bool Socket::IsNonBlock() const
{

}

size_t Socket::RecvBufSize() const
{

}