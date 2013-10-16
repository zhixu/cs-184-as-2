#include "shapes.h"
#include "primitives.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

Sphere::Sphere (Point p, float radius) {
    c = p;
    r = radius;
}


bool Sphere::intersect (Ray ray, float &thit, LocalGeo &local) {
    
    //printf("circle middle: x %f  y %f  z %f  radius: %f\n", c.x, c.y, c.z, r);
    Point e = ray.position + ray.direction * ray.t_min;
    Vector d = ray.direction;
    
    float A = d.dot(d);
    float B = 2*(d.dot(e-c));
    float C = (e-c).dot(e-c)-r*r;
    
    float disc = B*B - 4*A*C;
    
    if (disc <= 0) {
        //printf("no intersection\n");
        return 0;
    }
    
    disc = sqrt(pow(d.dot(e-c), 2) - d.dot(d)*((e-c).dot(e-c)-pow(r, 2)));
    float pt1 = -(d.dot(e-c));
    float denom = d.dot(d);
    
    float t1 = (pt1 + disc)/denom;
    float t2 = (pt1 - disc)/denom;
    
    if (t1 < t_min && t2 < t_min && t1 > t_max && t2 > t_max) { return 0; }
    
    Point p1 = e + d*t1;
    Point p2 = e + d*t2;
    
    float distance1 = (p1-e).dot(p1-e);
    float distance2 = (p2-e).dot(p2-e);
    
    if (distance1 < distance2) {
        thit = t1;
        Point position = p1;
        Vector normal = (p1-c)/r;
        local = LocalGeo(position, normal);
    } else {
        thit = t2;
        Point position = p2;
        Vector normal = (p2-c)/r;
        //printf("normal x %f y %f z %f\n", normal.x, normal.y, normal.z);
        //Vector n = normal.normalize();
        //printf("normalized x %f y %f z %f\n", n.x, n.y, n.z);
        local = LocalGeo(position, normal);
    }

//    printf("(%f, %f, %f) disc=%f denom=%f\n", e.x, e.y, e.z, disc, denom);
//    printf("(%f, %f, %f)\n", ray.position.x, ray.position.y, ray.position.z);
    //printf("discriminant: %f\n", disc);
    return 1;
}

Triangle::Triangle() {

}

Triangle::Triangle(Point x, Point y, Point z) {

    p0 = x;
    p1 = y;
    p2 = z; 
    
    normal = (p1-p0).dot(p2-p0);
    
}

bool Triangle::intersect(Ray ray, float &t_hit, LocalGeo &locals) {
    float a2, b2, c2, d2, e2, f, g, h, i, j, k, l;
    
    Point e = ray->position;
    Vector d = ray->direction;
    
    a2 = a.x - b.x;
    b2 = a.y - b.y;
    c2 = a.z - b.z;
    d2 = a.x - c.x;
    e2 = a.y - c.y;
    f = a.z - c.z;
    g = d.x;
    h = d.y;
    i = d.z;
    j = a.x - e.x;
    k = a.y - e.y;
    l = a.z - e.z;
    
    float ei_hf = e2*i - h*f;
    float gf_di = g*f - d2*i;
    float dh_eg = d2*h - e2*g;
    
    float M = a2*ei_hf + b2*gf_di + c2*dh_eg;
    
    
    t = - (f*(a2*k - j*b2) + h*(j*c2 - a2*l) + g*(b2*l - e2*g))/M;
    if (t < ray.t_min || t > ray.t_max) { return 0; }
    
    float gamma = (j*ei_hf + k*gf_di + l*dh_eg)/M;
    if (gamma < 0 || gamma > 1) { return 0; }
    
    float beta = (j*ei_hf + k*gf_di + l*dh_eg)/M;
    if (beta < 0 || beta > 1 - gamma) { return 0; }
    
    
    return 1;
}
