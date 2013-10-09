#ifndef FILM_H
#define FILM_H

#include "primitives.h"
#include "sampler.h"

class Film {
    private:
        Color** colors;
    public:
        int width, height;
        Film();
        Film(int, int);
        //void commit (Sample&, Color&);
        void write (char*);
};
#endif
