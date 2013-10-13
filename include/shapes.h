#ifndef SHAPE_H
#define SHAPE_H

#include "primitives.h"

class Shape {
    
};

class Sphere : public Shape {
    public:
        Point c;
        float r;
        Sphere();
        Sphere(Point, float);
        void intersect(Ray ray, float* t_hit, LocalGeo* local);
        bool intersectP(Ray ray);
};


#endif
