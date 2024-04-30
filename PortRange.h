#include <string>

class PortRange
{
private:
    std::string ip;
    int start_port;
    int end_port;
public:
    PortRange(const std::string& ip1, int start_port1, int end_port1) : 
        ip(ip1), start_port(start_port1), end_port(end_port1){} 
    bool is_valid()
    {
        return start_port >= end_port || start_port < 0 || end_port > 65535;
    }
};