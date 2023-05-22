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
	const int MAXREADSIZE = 1015;
	const int BUFSIZE = MAXREADSIZE + 9;
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
	
	recv(server,buf,1,0);
	if(buf[0] == 1){
	
	CHAR_SIZE_T readSize;
	readSize.a = MAXREADSIZE;
	buf[0]=1;
	while(server >=0){
		cout << min((int)(fileSize.a-readFile.tellg()),BUFSIZE) << endl;
		readSize.a = min((size_t)MAXREADSIZE,fileSize.a-readFile.tellg());
		memcpy(buf+1,readSize.b,8);
		readFile.read(buf+9,readSize.a);
		if(readSize.a != MAXREADSIZE){
			buf[0] = 0;
		}
		send(server,buf,BUFSIZE,0);
	}
	}
	readFile.close();
	close(server);
}
