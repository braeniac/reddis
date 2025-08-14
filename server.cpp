#include <iostream>      // For C++ I/O
#include <cstring>       // For memset, strerror
#include <unistd.h>      // For close()
#include <sys/types.h>   // For data types
#include <sys/socket.h>  // For socket(), bind(), listen(), accept()
#include <netinet/in.h>  // For sockaddr_in
#include <arpa/inet.h>   // For inet_addr(), htons()


int main() { 

    int val = 1; 
    //AF_INET = IPv4, SOCK_STREAM = TCP  
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    //socket, level, ,option value, size of option value
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); 

    return 0; 
}