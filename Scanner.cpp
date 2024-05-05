
#include <vector>
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

        
    std::vector<int> sockets;

    return 0;
}
