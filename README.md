# Network Programming (C++)
***
-`Note` remember attach ws2_32.lib to linker --> input
##Greating 
- Introducing Winsock2.h
- Program act like client get information address IPv4 and connect to server.
***
##Client and server
- Programming about TCP client, TCP server, UDP client and UDP server.
- Server is still hangging on one client (this problem will resolve in the next chap)
- A program (in TCPClien file) receive command-line argument (ex: TCPClient <IPv4/domain_name> port) act like a client connect to specificed server.
***
##ProgramWithThread
- Programming a server can receive, sent (connect) to some client (programming with thread).
- A server program act like telnet receive command-line argument (ex: mytelnets <port>). It can check username and password, and process command from client and return to client some result.
