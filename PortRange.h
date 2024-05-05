#include <string>
#include <functional>

class PortRange
{
private:
    std::string ip;
    int start_port;
    int end_port;
public:
    PortRange() = default;
    PortRange(const std::string& ip1, int start_port1, int end_port1) : 
        ip(ip1), start_port(start_port1), end_port(end_port1){} 
    bool is_valid()
    {
        return start_port < end_port && start_port > 0 && end_port < 65535;
    }
    std::string get_ip() const
    {
        return ip;
    }
    std::string& get_ip()
    {
        return ip;
    }
    void foreach(std::function<void(int)> fun)
    {
        for(int i = start_port; i <= end_port; ++i)
            fun(i);
    }
};