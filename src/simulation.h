#ifndef SIMULATION_H
#define SIMULATION_H

#include "defs.h"
#include "bin.h"
#include "atom.h"

#include "util/weakset.h"

class Simulation
{
public:
    Simulation(unsigned int size_x, unsigned int size_y)
        : size_x(size_x)
        , size_y(size_y)
        , bins_x(size_x / BIN_SIZE)
        , bins_y(size_y / BIN_SIZE)
        , bins(new Bin[bins_x * bins_y])
    {}

    unsigned int size_x;
    unsigned int size_y;

    void add_atom(Atom &atom);

    void step(float time);

protected:
    unsigned int bins_x;
    unsigned int bins_y;

    // On move, write (rad + med_rad)
    // On test, test (rad - med_rad)
    float med_rad = 20.0;

    Bin *bins;
    WeakSet<Atom> atoms;

    /*
    Bin *get_bin(float x, float y)
    {
        unsigned int bin_x = x / BIN_SIZE;
        unsigned int bin_y = y / BIN_SIZE;

        return bins + bin_x + bin_y * bins_x;
    }
    */

};

#endif // SIMULATION_H
