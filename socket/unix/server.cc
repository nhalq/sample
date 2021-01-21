#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

int main(const int argc, char const** argv)
{
    static const int port = 8080;
    static const int num_conns = 8;

    int option = 1;
    int server_fd, client_fd;

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        throw "socket::socket";

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)))
        throw "socket::setsockopt";

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw "socket::bind";

    if (listen(server_fd, num_conns) < 0)
        throw "socket::listen";

    while (true)
    {
        if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
            throw "socket::accept";

        size_t bufflen = 1024;
        char* buffer = new char[bufflen];
        const char* msg = "pong";

        size_t recvlen = recv(client_fd, buffer, bufflen, 0);
        send(client_fd, msg, strlen(msg), 0);
        printf("client: %s\n", buffer);
    }

    return 0;
}
