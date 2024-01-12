#include "Acceptor.h"

#include <fcntl.h>
#include <utility>
#include "Channel.h"
#include "Socket.h"
#include <cassert>
#include <iostream>

Acceptor::Acceptor(EventLoop *loop) {
  socket_ = std::make_unique<Socket>();
  assert(socket_->Create() == RS::RS_SUCCES);
  assert(socket_->Bind("127.0.0.1", 1234) == RS::RS_SUCCES);
  assert(socket_->Listen() == RS::RS_SUCCES);

  channel_ = std::make_unique<Channel>(socket_->fd(), loop);
  std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);

  channel_->set_read_callback(cb);
  channel_->EnableRead();
}

Acceptor::~Acceptor() {}

RS Acceptor::AcceptConnection() const{
  int clnt_fd = -1;
  if( socket_->Accept(clnt_fd) != RS::RS_SUCCES ) {
    //return RC_ACCEPTOR_ERROR;
  }
  // TODO: remove
  std::cout<<"new connection\n";
  fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);  // 新接受到的连接设置为非阻塞式
  if (new_connection_callback_) {
    new_connection_callback_(clnt_fd);
  }
  return RS::RS_SUCCES;
}

void Acceptor::set_new_connection_callback(std::function<void(int)> const &callback) {
    new_connection_callback_ = std::move(callback);
}