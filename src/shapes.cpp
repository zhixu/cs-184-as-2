#include "shapes.h"
#include "primitives.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

Sphere::Sphere (Point p, float radius) {
    c = p;
    r = radius;
}

bool Sphere::intersect (Ray ray, float* thit, LocalGeo* local) {
    
    Point e = ray.position;
    Vector d = ray.direction;
    
    float disc = sqrt(pow(d.dot(e-c), 2) - d.dot(d)*((e-c).dot(e-c)-pow(r, 2)));
    float pt1 = -d.dot(e-c);
    float denom = d.dot(d);
    
    float t1 = (pt1 + disc)/denom;
    float t2 = (pt1 - disc)/denom;
    
    Point p1 = e + d*t1;
    Point p2 = e + d*t2;
    
    float distance1 = (d*t1).dot(d*t1);
    float distance2 = (d*t2).dot(d*t2);
    
    if (distance1 < distance2) {
        *thit = t1;
        Point position = p1;
        Vector normal = (p1-c)/r;
        local = new LocalGeo(position, normal);
    } else {
        *thit = t2;
        Point position = p2;
        Vector normal = (p2-c)/r;
        local = new LocalGeo(position, normal);
    }
}

bool Sphere::intersectP (Ray ray) {
    
    Vector d = ray.direction;
    Point e = ray.position;
    
    float A = d.dot(d);
    float B = 2*(d.dot(e-c));
    float C = (e-c).dot(e-c)-r*r;
    
    float disc = B*B - 4*A*C;
    
    if (disc >= 0) {
        return 1;
    } else {
        return 0;
    }
}
