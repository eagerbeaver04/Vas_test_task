#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include "PortRange.h"
class EpollEvent
{
private:
    PortRange port_range;
    struct epoll_event event;
public: 
    explicit EpollEvent(PortRange port_range1) : port_range(port_range1)
    {
        event.events = EPOLLIN | EPOLLOUT;
        event.data.fd = -1;
    }
    EpollEvent(const std::string &ip1, int start_port1, int end_port1) 
    {
        port_range = {ip1, start_port1, end_port1};
        event.events = EPOLLIN | EPOLLOUT;
        event.data.fd = -1;
    }

    EpollEvent() = default;

    bool is_valid();
};