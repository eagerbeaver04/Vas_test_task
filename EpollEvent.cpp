#include "EpollEvent.h"

int set_nonblock(int fd)
{
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

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

std::vector<int> EpollEvent::get_open_ports()
{
    std::vector<int> open_ports;
    sockaddr_in serv_addr;

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return open_ports;
    }

    auto is_socket_valid = [this, &serv_addr, &open_ports, epoll_fd](int port) -> void
    {
        int sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (sock_fd == -1)
        {
            std::cerr << "Failed to create socket for port " << port << std::endl;
            return;
        }
        
        std::memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, port_range.get_ip().c_str(), &serv_addr.sin_addr) <= 0)
        {
            std::cerr << "Invalid address or address not supported" << std::endl;
            close(sock_fd);
            return;
        }

        if (connect(sock_fd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 && errno != EINPROGRESS)
        {
            std::cerr << "Failed to connect" << std::endl;
            close(sock_fd);
            return;
        }

        epoll_event event;
        event.events = EPOLLOUT | EPOLLERR;
        event.data.fd = sock_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &event) == -1)
        {
            std::cerr << "Failed to add socket to epoll instance" << std::endl;
            close(sock_fd);
            return;
        }

        int error = 0;
        socklen_t len = sizeof(error);
        if (!(getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0))
        {
            close(sock_fd);
            return;
        }
        open_ports.push_back(port);
    };

    port_range.foreach (is_socket_valid);

    return open_ports;
}