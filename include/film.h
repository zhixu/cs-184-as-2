#ifndef FILM_H
#define FILM_H

#include "primitives.h"
#include "sampler.h"
#include <vector>

class Sample;

class Film {
    private:
        std::vector< std::vector<Color*> > colors;
    public:
        int width, height;
        Film();
        Film(int, int);
        void commit (int, int, Color*);
        void write (char*);
};
#endif
