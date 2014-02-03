#ifndef BIN_H
#define BIN_H

#include "atom.h"

#include "util/weakset.h"

class Bin
{
public:
    WeakSet<Atom*> atoms;

    void remove_atom(Atom *atom)
    {
        WeakSet<Atom*>::iterator i = atoms.begin();
        while (i != atoms.end())
        {
            if (*i == atom)
            {
                atoms.erase(i);
                break;
            }
            i++;
        }
    }

    void add_atom(Atom *atom)
    {
        atoms.insert(atom);
    }
};

#endif // BIN_H
