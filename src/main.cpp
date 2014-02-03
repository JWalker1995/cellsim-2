#include <iostream>

#include "simulation.h"

int main()
{
    std::cout << "Creating simulation..." << std::endl;
    Simulation sim(1024, 512);

    std::cout << "Ending sucessfully..." << std::endl;
    return 0;
}
