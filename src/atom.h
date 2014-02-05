#ifndef ATOM_H
#define ATOM_H

class Atom
{
public:
    enum {t_membrane, t_acid, t_uranium, t_enzyme, t_carbon, t_ribosome, t_neuron, t_gold} type;

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

    static void collide(Atom *a1, Atom *a2);
};

#endif // ATOM_H
