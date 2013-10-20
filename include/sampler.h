#ifndef SAMPLER_H
#define SAMPLER_H

#include "primitives.h"
#include "film.h"

class Film;

class Sample {
    public:
        Point lookFrom, lookAt;
        Vector up;
        float fov;
        int width, height;
        Sample();
        Sample(Point, Point, Vector, float, Film);
        Point getSample(int, int);
        Vector ll, ul, lr, ur;
};    

#endif
