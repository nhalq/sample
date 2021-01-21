#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    static const int port = 8080;

	int client_fd = 0, valread;
	struct sockaddr_in serv_addr;

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw "socket::socket";

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port 	 = htons(port);
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
		throw "inet::inet_pton";

	if (connect(client_fd, (struct sockaddr*)(&serv_addr), sizeof(serv_addr)) < 0)
		throw "socket::connect";

	const char* msg = "ping";
	send(client_fd, msg, strlen(msg), 0 );
	printf("send: %s\n", msg);

	int bufflen = 1024;
	char* buffer = new char[bufflen];
	valread = recv(client_fd, buffer, bufflen, 0);
	printf("recv: %s\n", buffer);

	return 0;
}
