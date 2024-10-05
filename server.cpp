#include<iostream>
#include<thread>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include "wqueue.h"
#include "Tcp_acceptor.h"
using namespace std;
class WorkItem{
  TCPStream* m_stream;

  public:
    WorkItem(TCPStream* stream):m_stream(stream){}
    
    ~WorkItem(){delete m_stream;}
    
    TCPStream* get_stream(){
      return m_stream;
    } 
};

class ConnectionHandler
{
  wqueue<WorkItem*>& m_queue;
  
  public: 
    ConnectionHandler(wqueue<WorkItem*> & queue):m_queue(queue){}
    void* run()
    {
      for(int i=0;;i++)
      {
        cout<<"thread"<<std::this_thread::get_id()<<"loop"<<i<<" - Working for Item .. \n";
        WorkItem* item =m_queue.remove();
        if (item == NULL)return nullptr;
        cout<<"thread"<<std::this_thread::get_id()<<"loop"<<i<<" - Got one Item .. \n";
        TCPStream *stream =item->get_stream();
        char input[256];
        int len;
        while((len = stream->receive(input,sizeof(input)-1))>0)
        {
          input[len] ='\0';
          stream->send(input,len);
          cout<<"thread: "<<std::this_thread::get_id()<<"Echoed:"<<i<<" - Back to the Client .. \n";
        }
        delete item;
      } 
      return NULL;
    }
};

int main(int argc, char** argv)
{
   if(argc<3 || argc>4)
   {
     printf("usage:%s <worker><port><ip>\n",argv[0]);
     exit(-1);
   }
   int worker = atoi(argv[1]);
   int port = atoi(argv[2]);
   string ip;
   if(argc ==4)
   {
        ip = argv[3];
   }
   wqueue<WorkItem*> queue;
   ConnectionHandler* connection_handler = new ConnectionHandler(queue);
   std::vector<std::thread> workers;

   for(int i=0;i<worker;i++)
   { 
         workers.push_back(std::thread(&ConnectionHandler::run,std::ref(connection_handler)));
         
   }

  WorkItem* item;
  TCPAcceptor* connectionAcceptor;
  if (ip.length() > 0) {
      connectionAcceptor = new TCPAcceptor(port, (char*)ip.c_str());
  }
  else {
      connectionAcceptor = new TCPAcceptor(port);        
  }
  if (!connectionAcceptor || connectionAcceptor->start() > 0) {
        printf("Could not create an connection acceptor\n");
        exit(1);
  }
  int n =6;
  while (n--) {
        TCPStream* connection = connectionAcceptor->accept();
        if (!connection) {
            printf("Could not accept a connection\n");
            continue;
        }
        item = new WorkItem(connection);
        if (!item) {
            printf("Could not create work item a connection\n");
            continue;
        }
        queue.add(item);
  } 

  for(auto& thread:workers)
  {
    thread.join();
  }


return 0;
}
