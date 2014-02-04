#ifndef SIMULATION_H
#define SIMULATION_H

// Debug
#include <iostream>

#include "defs.h"
#include "bin.h"
#include "atom.h"

#include "util/weakset.h"

class Simulation
{
public:
    Simulation(float size_x, float size_y)
        : size_x(size_x)
        , size_y(size_y)
        , bins_x(size_x / BIN_SIZE)
        , bins_y(size_y / BIN_SIZE)
        , bins(new Bin[bins_x * bins_y])
    {}

    float size_x;
    float size_y;

    void add_atom(Atom atom_def);

    void step(float time);

protected:
    unsigned int bins_x;
    unsigned int bins_y;

    // On move, write (rad + med_rad)
    // On test, test (rad - med_rad)
    float med_rad = 20.0;

    Bin *bins;
    WeakSet<Atom> atoms;

    void bounce(Atom *a1, Atom *a2, float dx, float dy, float dsq, float r);
};

#endif // SIMULATION_H
