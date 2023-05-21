#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>

using namespace std;


union CHAR_SIZE_T{
	size_t a;
	char b[8];
};

int main(int argc,char **argv){
	int client,server;
	const int PORT = 42069;
	bool exit;
	int BUFSIZE = 1024;
	char buf[BUFSIZE];

	struct sockaddr_in server_addr;
	socklen_t size;


	client  =socket(AF_INET,SOCK_STREAM,0);
	if(client < 0){
		 cout << "err on socket" << endl;
		 return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(client,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
		cout << "error on bind" << endl;
		return 1;
	}

	size = sizeof(server_addr);

	listen(client,1);

	server = accept(client,(struct sockaddr*)&server_addr,&size);

	if(server <0){
		cout << "err on accept" << endl;
		return 1;
	}
	
	ifstream readFile(argv[1],ios_base::binary);
	readFile.seekg(0,ios::end);
	CHAR_SIZE_T fileSize;
	fileSize.a = readFile.tellg();
	readFile.seekg(0,ios::beg);
	
	memcpy(buf,fileSize.b,8);
	
	CHAR_SIZE_T fileNameSize;
	fileNameSize.a = strlen(argv[1]);

	memcpy(buf+8,fileNameSize.b,8);

	memcpy(buf+16,argv[1],fileNameSize.a);
	
	send(server,buf,BUFSIZE,0);

	while(server >=0){
		cout << "hello";
		readFile.read(buf+1,BUFSIZE-1);
		if(readFile.tellg() == fileSize.a)buf[0] = 0;
		else buf[0] = 1;
		send(server,buf,BUFSIZE,0);
	}
	readFile.close();
	close(server);
}
