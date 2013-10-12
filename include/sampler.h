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
        Vector x, y, z, u, v;
        int width, height;
        Sample();
        Sample(Point, Point, Vector, float, Film);
        Point getSample(int, int);

};    

#endif
