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

std::vector<int> EpollEvent::get_open_ports()
{
    std::vector<int> sockets;
    sockaddr_in serv_addr;
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return sockets;
    }

    auto is_socket_valid = [this, &serv_addr, &sockets, epoll_fd](int port) -> void
    {
        int sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (sock_fd == -1)
            return;

        std::memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, port_range.get_ip().c_str(), &serv_addr.sin_addr) <= 0)
        {
            close(sock_fd);
            return;
        }

        if (connect(sock_fd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 && errno != EINPROGRESS)
        {
            close(sock_fd);
            return;
        }
        epoll_event event;
        event.events = EPOLLIN || EPOLLOUT;
        event.data.fd = sock_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &event) == -1)
        {
            close(sock_fd);
            return;
        }
        struct epoll_event events[20];
        int nfds = epoll_wait(epoll_fd, events, 20, 20);
        if (nfds == -1)
        {
            close(sock_fd);
            return;
        }
        for (int i = 0; i < nfds; ++i)
        {

            if (((events[i].events & EPOLLOUT) || (events[i].events & EPOLLIN)))
            {
                sockets.push_back(port);
                break;
            }
        }
        close(sock_fd);
    };

    port_range.foreach (is_socket_valid);
    close(epoll_fd);

    return sockets;
}