#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "primitives.h"
#include "film.h"
#include "shapes.h"
#include <vector>

class RayTracer {
    private:
        std::vector< Shape > shapes;
        std::vector< Light > lights;
    public:
        RayTracer(std::vector< Shape >, std::vector< Light >);
        void trace(Ray, int, Color&);
        void illuminate(Color&, Point, LocalGeo, Brdf, Light);
};
#endif
