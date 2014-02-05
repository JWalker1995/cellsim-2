#include "simulation.h"

#include <math.h>

void Simulation::add_atom(Atom atom_def)
{
    atoms.insert(atom_def);
    Atom &atom = atoms.last();

    float rad = atom.rad + med_rad;

    atom.bin_sx = (atom.x - rad) / BIN_SIZE;
    atom.bin_ex = (atom.x + rad) / BIN_SIZE;
    atom.bin_sy = (atom.y - rad) / BIN_SIZE;
    atom.bin_ey = (atom.y + rad) / BIN_SIZE;

    if (atom.bin_sx >= bins_x) {atom.bin_sx = 0;}
    if (atom.bin_ex >= bins_x) {atom.bin_ex = bins_x - 1;}
    if (atom.bin_sy >= bins_y) {atom.bin_sy = 0;}
    if (atom.bin_ey >= bins_y) {atom.bin_ey = bins_y - 1;}

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

void Simulation::tick(float time)
{
    WeakSet<Atom>::iterator i = atoms.begin();
    while (i != atoms.end())
    {
        Atom &a = *i;

        a.x += a.vx;
        a.y += a.vy;

        // Bounce off walls
        float mx = size_x - a.rad;
        if (a.x > mx) {a.x = mx * 2.0 - a.x; a.vx = -abs(a.vx);}
        else if (a.x < a.rad) {a.x = a.rad * 2.0 - a.x; a.vx = abs(a.vx);}

        float my = size_y - a.rad;
        if (a.y > my) {a.y = my * 2.0 - a.y; a.vy = -abs(a.vy);}
        else if (a.y < a.rad) {a.y = a.rad * 2.0 - a.y; a.vy = abs(a.vy);}

        // Writing
        // TODO: cache in atom?
        float rad = a.rad + med_rad;

        unsigned int bin_sx = (a.x - rad) / BIN_SIZE;
        unsigned int bin_ex = (a.x + rad) / BIN_SIZE;
        unsigned int bin_sy = (a.y - rad) / BIN_SIZE;
        unsigned int bin_ey = (a.y + rad) / BIN_SIZE;

        if (bin_sx >= bins_x) {bin_sx = 0;}
        if (bin_ex >= bins_x) {bin_ex = bins_x - 1;}
        if (bin_sy >= bins_y) {bin_sy = 0;}
        if (bin_ey >= bins_y) {bin_ey = bins_y - 1;}

        /*
        remove_atom_from_col(*i, a.bin_sx, bin_sx, bin_sy, bin_ey);
        remove_atom_from_col(*i, a.bin_ex, bin_ex, bin_sy, bin_ey);
        remove_atom_from_row(*i, a.bin_sy, bin_sy, bin_sx, bin_ex);
        remove_atom_from_row(*i, a.bin_ey, bin_ey, bin_sx, bin_ex);
        */

        while (a.bin_sx < bin_sx)
        {
            unsigned int y = a.bin_sy;
            while (y <= a.bin_ey)
            {
                (bins + a.bin_sx + y * bins_x)->remove_atom(&a);
                y++;
            }
            a.bin_sx++;
        }
        while (a.bin_sx > bin_sx)
        {
            unsigned int y = a.bin_sy;
            while (y <= a.bin_ey)
            {
                (bins + a.bin_sx + y * bins_x)->add_atom(&a);
                y++;
            }
            a.bin_sx--;
        }

        while (a.bin_ex < bin_ex)
        {
            unsigned int y = a.bin_sy;
            while (y <= a.bin_ey)
            {
                (bins + a.bin_sx + y * bins_x)->add_atom(&a);
                y++;
            }
            a.bin_ex++;
        }
        while (a.bin_ex > bin_ex)
        {
            unsigned int y = a.bin_sy;
            while (y <= a.bin_ey)
            {
                (bins + a.bin_sx + y * bins_x)->remove_atom(&a);
                y++;
            }
            a.bin_ex--;
        }

        while (a.bin_sy < bin_sy)
        {
            unsigned int x = a.bin_sx;
            while (x <= a.bin_ex)
            {
                (bins + x + a.bin_sy * bins_x)->remove_atom(&a);
                x++;
            }
            a.bin_sy++;
        }
        while (a.bin_sy > bin_sy)
        {
            unsigned int x = a.bin_sx;
            while (x <= a.bin_ex)
            {
                (bins + x + a.bin_sy * bins_x)->add_atom(&a);
                x++;
            }
            a.bin_sy--;
        }

        while (a.bin_ey < bin_ey)
        {
            unsigned int x = a.bin_sx;
            while (x <= a.bin_ex)
            {
                (bins + x + a.bin_sy * bins_x)->add_atom(&a);
                x++;
            }
            a.bin_ey++;
        }
        while (a.bin_ey > bin_ey)
        {
            unsigned int x = a.bin_sx;
            while (x <= a.bin_ex)
            {
                (bins + x + a.bin_sy * bins_x)->remove_atom(&a);
                x++;
            }
            a.bin_ey--;
        }

        // Reading
        rad = a.rad - med_rad;
        bin_sx = (a.x - rad) / BIN_SIZE;
        bin_ex = (a.x + rad) / BIN_SIZE;
        bin_sy = (a.y - rad) / BIN_SIZE;
        bin_ey = (a.y + rad) / BIN_SIZE;

        unsigned int x = bin_sx;
        while (x <= bin_ex)
        {
            unsigned int y = bin_sy;
            while (y <= bin_ey)
            {
                WeakSet<Atom*> &bin_atoms = (bins + x + y * bins_x)->atoms;
                WeakSet<Atom*>::iterator j = bin_atoms.begin();
                while (j != bin_atoms.end())
                {
                    if (&a != *j)
                    {
                        float dx = (*j)->x - a.x;
                        float dy = (*j)->y - a.y;
                        float dsq = dx * dx + dy * dy;
                        float r = a.rad + (*j)->rad;

                        if (dsq < r * r)
                        {
                            // Colliding
                            bounce(&a, *j, dx, dy, dsq, r);
                            //runReaction(a, false);
                            //a->runReaction(this, true);
                            //collide(a);
                            //a->collide(this);
                        }
                    }
                    j++;
                }
                y++;
            }
            x++;
        }

        i++;
    }
}

void Simulation::render(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    WeakSet<Atom>::iterator i = atoms.begin();
    while (i != atoms.end())
    {
        Atom &a = *i;

        i++;
    }
}

void Simulation::bounce(Atom *a1, Atom *a2, float dx, float dy, float dsq, float r)
{
    // Calculate the number of timesteps to move each atom back.
    float dvx = a2->vx - a1->vx;
    float dvy = a2->vy - a1->vy;
    float dvx_sq = dvx * dvx;
    float dvy_sq = dvy * dvy;
    float dx_dvx = dx * dvx;
    float dy_dvy = dy * dvy;
    float t = (dx_dvx + dy_dvy + sqrt(2.0 * dx_dvx * dy_dvy - dvx_sq * dy * dy - dvy_sq * dx * dx + (dvy_sq + dvx_sq) * dsq)) / (dvx_sq + dvy_sq);

    // Moves the atom back t timesteps, so that the atoms are at the instant of the collision.
    // Then, we run the bounce function, which changes the velocities of each atom.
    // Then, we move each atom forward t timesteps.

    a1->x -= a1->vx * t;
    a1->y -= a1->vy * t;
    a2->x -= a2->vx * t;
    a2->y -= a2->vy * t;
    dx = a2->x - a1->x;
    dy = a2->y - a1->y;
    float ax = dx / r;
    float ay = dy / r;

    float p = 2 * (a1->vx * ax + a1->vy * ay - a2->vx * ax - a2->vy * ay) / (a1->mass + a2->mass);
    a1->vx -= p * a1->mass * ax;
    a1->vy -= p * a1->mass * ay;
    a2->vx += p * a2->mass * ax;
    a2->vy += p * a2->mass * ay;

    a1->x += a1->vx * t;
    a1->y += a1->vy * t;
    a2->x += a2->vx * t;
    a2->y += a2->vy * t;
}
