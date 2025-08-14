#include <iostream>      // For C++ I/O
#include <cstring>       // For memset, strerror
#include <unistd.h>      // For close()
#include <sys/types.h>   // For data types
#include <sys/socket.h>  // For socket(), bind(), listen(), accept()
#include <netinet/in.h>  // For sockaddr_in
#include <arpa/inet.h>   // For inet_addr(), htons()

void msg(const char *msg) {
   fprintf(stderr, "%s\n", msg);
}

static void doSomething(int connfd) {

    //read & write 
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1 ); 
    if (n < 0) {
        msg("read() error"); 
        return; 
    }

    printf("client says: %s\n", rbuf);

    char wbuf[] = "World!";
    write(connfd, wbuf, strlen(wbuf));
}

void die(const char *msg) {
    int err = errno; 
    fprintf(stderr, "[%d] %s\n", err, msg);
    exit(EXIT_FAILURE);
}

int main() { 

    int opt = 1; 
    //AF_INET = IPv4, SOCK_STREAM = TCP  
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        die("socket()");
    }

    //socket, level, ,option value, size of option value
    //SO_REUSEADDR level allows OS to resuse ip+port
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 


    //wildcard address 0.0.0.0:1234
    //sockaddr_in for IPv4 and sockaddr_in6 for IPv6
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;          //IPv4
    addr.sin_port = htons(1234);        //PORT in big-endian
    addr.sin_addr.s_addr = htonl(0);    //wildcard ip: 0.0.0.0

    //bind
    //rv either returns a 0  (sucess) or -1 (error)
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()"); 
    }

    //listen
    //socket is created 
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()"); 
    }

    //accept 
    while (true) {
        struct sockaddr_in client_addr = {}; 
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;
        }
        doSomething(connfd); 
        close(connfd); 
    }

    return 0; 
}