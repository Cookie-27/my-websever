#include "Socket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "common.h"
#include <sys/ioctl.h>
#include <fcntl.h>

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
    if (listen(fd_, SOMAXCONN) == -1) {
        //throw std::runtime_error("Failed to listen on the socket");
    }

    // 设置监听状态成功，返回成功状态或其他适当的值
    return RS::RS_SUCCES;
}

RS Socket::Accept(int &clnt_fd) const
{

    clnt_fd = accept(fd_, NULL, NULL);

    if (clnt_fd == -1) {
        // 接受连接失败，可以根据实际情况处理错误
        //throw std::runtime_error("Failed to accept client connection");
        return RS::RS_DEFEAT;
    }

    // 接受连接成功，返回成功状态或其他适当的值
    return RS::RS_SUCCES;
}

RS Socket::Connect(const char *ip, uint16_t port) const
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if (::connect(fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to connect socket");
        return RS::RS_DEFEAT;
    }
    return RS::RS_SUCCES;
}

RS Socket::SetNonBlock() const {
  return SetBlockInternal(true);
}
RS Socket::SetBlock() const{
  return SetBlockInternal(false);
}

RS Socket::SetBlockInternal(bool non_block) const {
  int flags;
  if ((flags = fcntl(fd_, F_GETFL)) == -1) {
    //perror("Socket fcntl(F_GETFL) error");
    return RS::RS_DEFEAT;
  }
  if (!!(flags & O_NONBLOCK) == !!non_block)
    return RS::RS_SUCCES;
  if (non_block) 
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;
  
  if (fcntl(fd_, F_SETFL, flags) == -1) {
    //perror("Socket fcntl(F_SETFL) error");
    //return RC_SOCKET_ERROR;
  }

  return RS::RS_SUCCES;
}

bool Socket::IsNonBlock() const
{
    return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0;
}

size_t Socket::RecvBufSize() const
{
    int size;
    socklen_t len = sizeof(size);

    // 查询套接字接收缓冲区大小
    if (getsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &size, &len) == -1) {
        // 查询失败，可以根据实际情况处理错误
        //throw std::runtime_error("Failed to get receive buffer size");
    }

    // 返回接收缓冲区大小
    return static_cast<size_t>(size);
}