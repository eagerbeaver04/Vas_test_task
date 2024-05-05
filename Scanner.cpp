#include "EpollEvent.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Incorrect number of input values\n";
        return 1; 
    }

    EpollEvent event(argv[1], std::atoi(argv[2]), std::atoi(argv[3]));

    if(!event.is_valid())
        return 1;

    std::vector<int> ports = event.get_open_ports();

    std::cout << "begin list of open ports:" << std::endl;
    for(int i =0; i < ports.size(); ++i)
        std::cout << ports[i] << std::endl;
    std::cout << "end list of open ports" << std::endl;

    return 0;
}
