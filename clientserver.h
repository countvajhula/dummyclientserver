#ifndef __CLIENTSERVER_H
#define __CLIENTSERVER_H


typedef struct {
	int sockfd;
	struct sockaddr_in theiraddr;
} conn_t;


conn_t* connect_tcp(const char* serverIP, const char* serverPort);
void disconnect_tcp(conn_t* connxn);
conn_t* listen_tcp(const char* listenPort);
conn_t* accept_tcp(conn_t* listenconn);
int recv_tcp(conn_t* connxn, char* buffer, unsigned int bufSize);
int send_tcp(conn_t* connxn, char* buffer, unsigned int bytesToSend);
int recvData(conn_t* connxn, char* buffer, unsigned int bufSize);
int sendData(conn_t* connxn, char* buffer, unsigned int bytesToSend);

#endif
