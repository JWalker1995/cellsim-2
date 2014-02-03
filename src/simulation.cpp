#include "simulation.h"

void Simulation::add_atom(Atom &atom)
{
    // Shouldn't have to copy here
    atoms.insert(atom);

    float rad = atom.rad + med_rad;

    atom.bin_sx = (atom.x - rad) / BIN_SIZE;
    atom.bin_ex = (atom.x + rad) / BIN_SIZE;
    atom.bin_sy = (atom.y - rad) / BIN_SIZE;
    atom.bin_ey = (atom.y + rad) / BIN_SIZE;

    unsigned int x = atom.bin_sx;
    while (x <= atom.bin_ex)
    {
        unsigned int y = atom.bin_sy;
        while (y <= atom.bin_ey)
        {
            (bins + x + y * bins_x)->add_atom(&atom);
            y++;
        }
        x++;
    }
}

void Simulation::step(float time)
{
    WeakSet<Atom>::iterator i = atoms.begin();
    while (i != atoms.end())
    {
        i->x += i->vx;
        i->y += i->vy;

        // TODO: cache in atom?
        float rad = i->rad + med_rad;

        unsigned int bin_sx = (i->x - rad) / BIN_SIZE;
        unsigned int bin_ex = (i->x + rad) / BIN_SIZE;
        unsigned int bin_sy = (i->y - rad) / BIN_SIZE;
        unsigned int bin_ey = (i->y + rad) / BIN_SIZE;

        /*
        remove_atom_from_col(*i, i->bin_sx, bin_sx, bin_sy, bin_ey);
        remove_atom_from_col(*i, i->bin_ex, bin_ex, bin_sy, bin_ey);
        remove_atom_from_row(*i, i->bin_sy, bin_sy, bin_sx, bin_ex);
        remove_atom_from_row(*i, i->bin_ey, bin_ey, bin_sx, bin_ex);
        */

        while (i->bin_sx < bin_sx)
        {
            unsigned int y = i->bin_sy;
            while (y <= i->bin_ey)
            {
                (bins + i->bin_sx + y * bins_x)->remove_atom(&*i);
                y++;
            }
            i->bin_sx++;
        }
        while (i->bin_sx > bin_sx)
        {
            unsigned int y = i->bin_sy;
            while (y <= i->bin_ey)
            {
                (bins + i->bin_sx + y * bins_x)->add_atom(&*i);
                y++;
            }
            i->bin_sx--;
        }

        while (i->bin_ex < bin_ex)
        {
            unsigned int y = i->bin_sy;
            while (y <= i->bin_ey)
            {
                (bins + i->bin_sx + y * bins_x)->add_atom(&*i);
                y++;
            }
            i->bin_ex++;
        }
        while (i->bin_ex > bin_ex)
        {
            unsigned int y = i->bin_sy;
            while (y <= i->bin_ey)
            {
                (bins + i->bin_sx + y * bins_x)->remove_atom(&*i);
                y++;
            }
            i->bin_ex--;
        }

        while (i->bin_sy < bin_sy)
        {
            unsigned int x = i->bin_sx;
            while (x <= i->bin_ex)
            {
                (bins + x + i->bin_sy * bins_x)->remove_atom(&*i);
                x++;
            }
            i->bin_sy++;
        }
        while (i->bin_sy > bin_sy)
        {
            unsigned int x = i->bin_sx;
            while (x <= i->bin_ex)
            {
                (bins + x + i->bin_sy * bins_x)->add_atom(&*i);
                x++;
            }
            i->bin_sy--;
        }

        while (i->bin_ey < bin_ey)
        {
            unsigned int x = i->bin_sx;
            while (x <= i->bin_ex)
            {
                (bins + x + i->bin_sy * bins_x)->add_atom(&*i);
                x++;
            }
            i->bin_ey++;
        }
        while (i->bin_ey > bin_ey)
        {
            unsigned int x = i->bin_sx;
            while (x <= i->bin_ex)
            {
                (bins + x + i->bin_sy * bins_x)->remove_atom(&*i);
                x++;
            }
            i->bin_ey--;
        }

        // Testing
        //float rad = i->rad - med_rad;

        i++;
    }
}
