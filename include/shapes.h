#ifndef SHAPE_H
#define SHAPE_H

#include "primitives.h"

class Shape {
    public:
        static Brdf brdf;
        virtual bool intersect(Ray ray, float &t_hit, LocalGeo &local) = 0;

};

class Sphere : public Shape {
    public:
        Point c;
        float r;
        Sphere();
        Sphere(Point, float);
        bool intersect(Ray ray, float &t_hit, LocalGeo &local);
};

class Triangle : public Shape {
    public:
        Point p0, p1, p2;
        Vector normal;
        Triangle();
        Triangle(Point, Point, Point);
        bool intersect(Ray ray, float &t_hit, LocalGeo &local);
};

#endif
