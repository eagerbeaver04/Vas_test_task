#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <vector>

#include "PortRange.h"

class EpollEvent
{
private:
    PortRange port_range;
    epoll_event event;
    int epoll_fd;
    bool is_epoll_fd_valid();
    bool is_port_range_valid();
public: 
    explicit EpollEvent(PortRange port_range1) :
     port_range(port_range1), event(), epoll_fd(epoll_create1(0))
    {
        if (epoll_fd == -1)
            std::cerr << "Failed to create epoll instance" << std::endl;
        event.events = EPOLLIN | EPOLLOUT;
        event.data.fd = -1;
    }
    EpollEvent(const std::string &ip1, int start_port1, int end_port1) :
     port_range({ip1, start_port1, end_port1}), event(), epoll_fd(epoll_create1(0))
    {
        if (epoll_fd == -1)
            std::cerr << "Failed to create epoll instance" << std::endl;
        event.events = EPOLLIN | EPOLLOUT;
        event.data.fd = -1;
    }

    EpollEvent() = default;
    ~EpollEvent() 
    {
        if(epoll_fd != -1)
            close(epoll_fd);
    }
    bool is_valid();
    std::vector<int> valid_sockets();
};