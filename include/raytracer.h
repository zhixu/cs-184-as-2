#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "primitives.h"
#include "film.h"
#include <vector>

class RayTracer {
    private:
        std::vector< Primitive* > primitives;
        std::vector< Light* > lights;
    public:
        RayTracer(std::vector< Primitive* >);
        void trace(Ray&, int, Color*);
        Color illuminate(LocalGeo, );
};
#endif