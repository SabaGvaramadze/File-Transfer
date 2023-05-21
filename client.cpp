#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fstream>

using namespace std;

union CHAR_SIZE_T{
	size_t a;
	char b[8] = {0};
};

int main(){
	int client,server;
	int PORT = 42069;
	const int BUFSIZE = 1024;
	char buf[BUFSIZE] = {0};
	char *ip="127.0.0.1";

	struct sockaddr_in server_addr;

	client = socket(AF_INET,SOCK_STREAM,0);
	
	if(client < 0){
		cout << "err on socket" << endl;
		return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);


	if(connect(client,(struct sockaddr*)&server_addr,sizeof(server_addr)) == 0){
		cout << "connecting" <<endl;
	}

	recv(client,buf,BUFSIZE,0);
	CHAR_SIZE_T fileSize;
	memcpy(fileSize.b,buf,8);
	CHAR_SIZE_T fileNameSize;
	memcpy(fileNameSize.b,buf+8,8);
	char fileName[fileNameSize.a +1] = {0};
	fileName[fileNameSize.a] = '\0';
	memcpy(fileName,buf+16,fileNameSize.a);
	string a = fileName;
	cout << "FILE SIZE: " << fileSize.a  << "bytes"<< endl;
	cout << "FILE NAME: " << fileName << endl;

	ofstream outFile("out.jpg",ios_base::binary);

	while(client<1){
		recv(client,buf,BUFSIZE,0);
		outFile.write(buf+1,BUFSIZE-1);
		if(buf[0] == 0){
			break;
		}
	}
	outFile.close();
	close(client);
		
}
