#include "EpollEvent.h"

bool EpollEvent::is_epoll_fd_valid()
{
    if (epoll_fd == -1)
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return false;
    }
    return true;
}

bool EpollEvent::is_port_range_valid()
{
    if (!port_range.is_valid())
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return false;
    }
    return true;
}

bool EpollEvent::is_valid()
{
    return !is_epoll_fd_valid() || !is_port_range_valid();
}
