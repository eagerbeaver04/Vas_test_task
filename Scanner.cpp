#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include "PortRange.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Incorrect number of input values\n";
        return 1; 
    }

    PortRange port {argv[1], std::atoi(argv[2]), std::atoi(argv[3])};

    if(!port.is_valid())
    {
        std::cerr << "Incorrect port range\n";
        return 1;
    }

    int epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return 1;
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLOUT; 
    event.data.fd = -1;

    std::vector<int> sockets;


    return 0;
}
