#include "Server.h"
#include "Acceptor.h"
#include <cassert>
#include "Connection.h"
#include "EventLoop.h"
#include <thread>
#include "ThreadPool.h"
#include "HttpContext.h"
#include "Buffer.h"
#include "HttpResponse.h"
#include "HttpRequset.h"
#include <iostream>
#include <fstream>
#include <sstream>

Server::Server(){
    main_reactor_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(main_reactor_.get());
    std::function<void(int)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);

    unsigned int size = std::thread::hardware_concurrency();
    thread_pool_ = std::make_unique<ThreadPool>(size);

    for (size_t i = 0; i < size; ++i) {
      std::unique_ptr<EventLoop> sub_reactor = std::make_unique<EventLoop>();
      sub_reactors_.push_back(std::move(sub_reactor));
    }
    //LOG_LEVEL(LOG_DEBUG);
}
Server::~Server(){};

RS Server::NewConnection(int fd) {
  assert(fd != -1);
  uint64_t random = fd % sub_reactors_.size();    //allocate

  std::unique_ptr<Connection> conn = std::make_unique<Connection>(fd, sub_reactors_[random].get());
  std::function<void(int)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  
  conn->set_delete_connection(cb);
  conn->set_on_recv(on_recv_);

  auto req_cb = std::bind(&Server::onRequest, this,std::placeholders::_1);
  conn->set_on_req(req_cb);
  
  connections_[fd] = std::move(conn);
  if (on_connect_) {
    on_connect_(connections_[fd].get());
  }
  //LOG_INFO("new connection fd: %d", fd);
  return RS::RS_SUCCES;
}

RS Server::DeleteConnection(int fd) {
  auto it = connections_.find(fd);
  assert( it != connections_.end() );
  connections_.erase(fd);
  return RS::RS_SUCCES;
}

void Server::onConnect(std::function<void(Connection *)> fn)
{
    on_connect_ = std::move(fn);
}

void Server::onRecv(std::function<void(Connection *)> fn) 
{ 
    on_recv_ = std::move(fn); 
}

void Server::Start(){
  for (size_t i = 0; i < sub_reactors_.size(); ++i) {
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i].get());
    thread_pool_->Add(std::move(sub_loop));
  }
  main_reactor_->Loop();
  //LOG_INFO("start server success!");
}

void Server::onRequest(Connection *conn)
{
  HttpContext *context = conn->getContext();
  if(context == nullptr){
    //error
  }
  if(!context->parseRequest(conn->read_buf())){
    //conn->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
  }
  if (context->gotAll())
  {
    HttpRequest req = context ->request();
    HttpResponse response(false);

    //set body
    std::string path = req.path();
    std::ifstream fileStream(path);
    if (!fileStream.is_open()) {
        //404 NOT FOUND
    }
    std::stringstream fileBuffer;
    fileBuffer << fileStream.rdbuf();
    response.setBody(fileBuffer.str());

    //set first head
    response.setStatusCode(HttpResponse::k200Ok);
    response.setContentType("html"); //default
    response.setCloseConnection(false);
    response.setStatusMessage("OK");

    Buffer buf;
    response.appendToBuffer(&buf);
    conn->Send(&buf);
    context->reset();
  }
}