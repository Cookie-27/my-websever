#include <string>
#include "common.h"

class Socket {
 public:
  //DISALLOW_COPY_AND_MOVE(Socket);
  Socket();
  ~Socket();
  void set_fd(int fd);
  int fd() const;
  std::string get_addr() const;
  RS Create();
  RS Bind(const char *ip, uint16_t port) const;
  RS Listen() const;
  RS Accept(int &clnt_fd) const;
  RS Connect(const char *ip, uint16_t port) const;
  RS SetNonBlock() const;
  RS SetBlock() const;
  bool IsNonBlock() const;
  size_t RecvBufSize() const;

 private:
  RS SetBlockInternal(bool non_block) const;
  int fd_;
};