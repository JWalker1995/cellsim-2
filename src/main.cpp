#include <iostream>

#include "window.h"

int main()
{
    std::cout << "Creating simulation..." << std::endl;
    Simulation *sim = new Simulation(1000, 500);

    std::cout << "Starting..." << std::endl;

    Window window(sim);
    bool res = window.run();

    std::cout << "Ending..." << std::endl;

    delete sim;

    return res;
/*
    std::cout << "Creating simulation..." << std::endl;
    Simulation sim(1024, 512);

    Atom atom;
    atom.x = 100.0;
    atom.y = 100.0;
    atom.vx = 1.0;
    atom.vy = 0.0;
    atom.rad = 20.0;
    atom.mass = 1.0;
    sim.add_atom(atom);

    atom.x = 140.0;
    atom.y = 120.0;
    atom.vx = -4.0;
    atom.vy = 0.0;
    atom.rad = 20.0;
    atom.mass = 1.0;
    sim.add_atom(atom);

    unsigned int i = 0;
    while (i < 100)
    {
        sim.step(0.01);

        i++;
    }

    std::cout << "Ending sucessfully..." << std::endl;
    return 0;
*/
}
