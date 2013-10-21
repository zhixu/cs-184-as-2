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
        Sample(Point, Point, Vector, float, Film, int);
        Point getSample(int, int);
        Vector w, u, v;
        float tanHalfY, tanHalfX;
        float halfWidth, halfHeight;
        int raysPerPixel;
};

#endif
