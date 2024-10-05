/*
   TCPConnector.h

   TCPConnector class interface. TCPConnector provides methods to actively
   establish TCP/IP connections with a server.
*/

#include <netinet/in.h>
#include "Tcp_stream.h"

class TCPConnector
{
  public:
    TCPStream* connect(const char* server, int port);
    TCPStream* connect(const char* server, int port, int timeout);
    
  private:
    int resolveHostName(const char* host, struct in_addr* addr);
};