#pragma once
#include "common.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

class Server {
 public:
  //DISALLOW_COPY_AND_MOVE(TcpServer);
  Server();
  ~Server();
  void Start();

  RS NewConnection(int fd);
  RS DeleteConnection(int fd);

  void onConnect(std::function<void(Connection *)> fn);
  void onRecv(std::function<void(Connection *)> fn);

 private:
  std::unique_ptr<EventLoop> main_reactor_;
  std::unique_ptr<Acceptor> acceptor_;

  std::unordered_map<int, std::unique_ptr<Connection>> connections_;
  std::vector<std::unique_ptr<EventLoop>> sub_reactors_;

  std::unique_ptr<ThreadPool> thread_pool_;

  std::function<void(Connection *)> on_connect_;
  std::function<void(Connection *)> on_recv_;
};