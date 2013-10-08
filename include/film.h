#include "primitives.h"

#ifndef FILM_H
#define FILM_H

class Film {
    private:
        Color** colors;
    public:
        int width, height;
        Film();
        Film(int, int);
        void commit (Sample&, Color&);
        void write (char*);
};
