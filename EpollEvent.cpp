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
        std::cerr << "Port range is not valid" << std::endl;
        return false;
    }
    return true;
}

bool EpollEvent::is_valid()
{
    return is_epoll_fd_valid() && is_port_range_valid();
}

std::vector<int> EpollEvent::valid_sockets()
{
    std::vector<int> sockets;
    sockaddr_in serv_addr;
    auto is_socket_valid = [this, &serv_addr, & sockets](int port) -> void
    {
        int sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (sock_fd == -1)
        {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        std::memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, port_range.get_ip().c_str(), &serv_addr.sin_addr) <= 0)
        {
            close(sock_fd);
            return;
        }

        if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 && errno != EINPROGRESS)
        {
            close(sock_fd);
            return;
        }

        event.data.fd = sock_fd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &event) == -1)
        {
            close(sock_fd);
            return;
        }
        
        close(sock_fd);
        sockets.push_back(port);
    };

    port_range.foreach(is_socket_valid);
    
    return sockets;

}