#ifndef ATOM_H
#define ATOM_H

class Atom
{
public:
    float x;
    float y;

    float vx;
    float vy;

    float rad;

    float mass;

    unsigned int bin_sx;
    unsigned int bin_ex;
    unsigned int bin_sy;
    unsigned int bin_ey;
};

#endif // ATOM_H
