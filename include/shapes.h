#ifndef SHAPE_H
#define SHAPE_H

class Sphere {
    public:
        Point c;
        float r;
        Sphere();
        Sphere(Point, float);
        bool intersect(Ray& ray, float* t_hit, LocalGeo* local);
        bool intersectP(Ray& ray);
};


#endif
