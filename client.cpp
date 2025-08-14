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


void die(const char *msg) {
    int err = errno; 
    fprintf(stderr, "[%d] %s\n", err, msg);
    exit(EXIT_FAILURE);
}

int main() {

    int opt = 1; 
    int fd = socket(AF_INET, SOCK_STREAM, 0); 

    if (fd < 0) {
        die("socket()"); 
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 

    struct sockaddr_in addr = {}; 
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1

    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr)); 
    if (rv) {
        die("connect"); 
    }

    char wbuf[] = "Hello"; 
    write(fd, wbuf, strlen(wbuf)); 

    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        die("read");
    }
    printf("server says: %s\n", rbuf);

    close(fd); 
    return 0; 
}