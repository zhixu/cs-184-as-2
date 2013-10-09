#include "shapes.h"
#include "primitives.h"
#include <glut.h>
#include <math.h>

Sphere::Sphere (Point p, float radius) {
    c = p;
    r = radius;
}

bool Sphere::intersect (Ray& ray, float* t_hit, LocalGeo* local) {
    
    float e = ray.position;
    float d = ray.direction;
    
    float disc = sqrt(pow(cross(d, e-c), 2) - cross(d, d)*(cross(e-c, e-c) - pow(r, 2)));
    float pt1 = -cross(d, d-c);
    float denom = cross(d, d);
    
    float t1 = (pt1 + disc)/denom;
    float t2 = (pt1 - disc)/denom;
    
    &t_hit = t1;
    &(t_hit+1) = t2;

}

bool Sphere::intersectP (Ray& ray) {
    
    float A = cross(ray.direction, ray.direction);
    float B = 2*(cross(ray.direction, ray.position-c));
    float C = cross(ray.position-c, ray.position-c);
    
    float disc = B*B - 4*A*C;
    
    if (disc >= 0) {
        return 1;
    } else {
        return 0;
    }
    
}

