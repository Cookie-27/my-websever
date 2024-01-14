#include "HttpResponse.h"
#include <string>

void HttpResponse::appendToBuffer(Buffer* output) const
{
  char buf[32];
  snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
  output->Append(buf,sizeof(buf));
  //output->Append(statusMessage_,sizeof(statusMessage_));
  output->Append("\r\n",2);

  if (closeConnection_)
  {
    //output->Append("Connection: close\r\n");
  }
  else
  {
    snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
    //output->Append(buf);
    //output->Append("Connection: Keep-Alive\r\n");
  }

  for (std::map<std::string, std::string>::const_iterator it = headers_.begin();
       it != headers_.end();
       ++it)
  {
    //output->append(it->first);
    //output->append(": ");
    //output->append(it->second);
    //output->append("\r\n");
  }

  //output->append("\r\n");
  //output->append(body_);
}