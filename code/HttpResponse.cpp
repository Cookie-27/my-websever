#include "HttpResponse.h"
#include <string>
#include <cstring>

void HttpResponse::appendToBuffer(Buffer* output) const
{
  char buf[32];
  snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
  output->Append(buf, strlen(buf));  // 使用 strlen 获取字符串长度

  output->Append(statusMessage_.c_str(), statusMessage_.size());  // 使用 c_str() 和 size() 获取字符串内容和长度
  output->Append("\r\n", 2);

  if (closeConnection_)
  {
    output->Append("Connection: close\r\n", 22);
  }
  else
  {
    snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
    output->Append(buf, strlen(buf));

    output->Append("Connection: Keep-Alive\r\n", 24);
  }

  for (std::map<std::string, std::string>::const_iterator it = headers_.begin();
       it != headers_.end();
       ++it)
  {
    output->Append(it->first.c_str(), it->first.size());
    output->Append(": ", 2);
    output->Append(it->second.c_str(), it->second.size());
    output->Append("\r\n", 2);
  }

  output->Append("\r\n", 2);
  output->Append(body_.c_str(), body_.size());
}