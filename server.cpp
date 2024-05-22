#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

void die(const char* message) {
    perror(message);
    exit(1);
}

static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        printf("read() error");
        return ;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("%d\n", fd);


    // bind, this is the syntax that deals with IPV4 addresses
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0

    int rv = bind(fd, (const sockaddr*)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen
    rv = listen (fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    while (true) {
        // accept
        struct sockaddr_in client_addr={};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;
        }

        do_something(connfd);
        close(connfd);
    }
}