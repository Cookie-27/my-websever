#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"

Connection::Connection(int fd, EventLoop *loop){
  socket_ = std::make_unique<Socket>();
  socket_->set_fd(fd);
  if (loop != nullptr) {
    channel_ = std::make_unique<Channel>(fd, loop);
    channel_->EnableRead();
    channel_->EnableET();
  }
  read_buf_ = std::make_unique<Buffer>();
  send_buf_ = std::make_unique<Buffer>();

  state_ = State::Connected;
}

Connection::~Connection(){}


void Connection::set_delete_connection(std::function<void(int)> const &fn)
{

}

void Connection::set_on_connect(std::function<void(Connection *)> const &fn)
{

}

void Connection::set_on_recv(std::function<void(Connection *)> const &fn)
{

}

Connection::State Connection::state() const
{
    
}

Socket* Connection::socket() const
{

}
  void set_send_buf(const char *str);
Buffer *read_buf();
Buffer *send_buf();

RS Connection::Read()
{
  if (state_ != State::Connected) {
    //perror("Connection is not connected, can not read");
    //return RC_CONNECTION_ERROR;
  }
  //assert(state_ == State::Connected && "connection state is disconnected!");
  read_buf_->Clear();
  if (socket_->IsNonBlock()) {
    return ReadNonBlocking();
  } else {
    return ReadBlocking();
  }
}

  RS Write();
  RS Send(std::string msg);

  void Close();

  void onConnect(std::function<void()> fn);
  void onMessage(std::function<void()> fn);

//private
void Business();
RS ReadNonBlocking();
RS WriteNonBlocking();
RS ReadBlocking();
RS WriteBlocking();