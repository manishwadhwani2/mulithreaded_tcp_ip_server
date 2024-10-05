
                                                      MUltithreaded TCP IP SERVER

Creating a multithreaded TCP/IP protocol based server requires the capabilities to handle network communication, multithreading and transferring data between threads.

The server is based on the producer-consumer multithreaded model . In our case the producer thread accepts connections then queues the connections for the consumer threads which in turn handle the connection processing.

The client simply makes a connection, sends a message to the server and waits for the server to echo it back. This action is performed twice. In both cases the message sent and received back is displayed

Build 

Run the  following command to compile server and client 

Server -: makefile -f Makefile.server

Client -: makefile -f Makefile.client

Run
 
First terminal - Run these command ./server 5 9999 localhost

Second Terminal - Run these command -: ./client 9999 localhost; ./client 9999 localhost; ./client 9999 localhost

Six messages, two by each client, are sent to the server.

Output 

First terminal 

<img width="719" alt="image" src="https://github.com/user-attachments/assets/b8440a0b-495e-449e-8701-a598e25f777d">


Second termminal 

<img width="917" alt="image" src="https://github.com/user-attachments/assets/1ec48d2c-c6b8-4d49-b25a-ab833d1975eb">



