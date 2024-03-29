#ifndef SHAPE_H
#define SHAPE_H

#include "primitives.h"

class Shape {
    public:
        Brdf brdf;
        Matrix objectToWorld, worldToObject;
        // worldToObject is what you multiply a Ray by to get it into the perspective of the unit sphere
        virtual bool intersect(Ray &ray, float &t_hit, LocalGeo &local) = 0;

};

class Sphere : public Shape {
    public:
        Point c;
        float r;
        Sphere();
        Sphere(Point, float);
        bool intersect(Ray &ray, float &t_hit, LocalGeo &local);
};

class Triangle : public Shape {
    public:
        Point a, b, c;
        Vector normal, normal1, normal2;
        Triangle();
        Triangle(Point, Point, Point);
        bool intersect(Ray &ray, float &t_hit, LocalGeo &local);
};

#endif
