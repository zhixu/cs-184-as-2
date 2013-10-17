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
    Point e = ray.position;
    Vector d = ray.direction;
    
    float A = d.dot(d);
    float B = 2*(d.dot(e-c));
    float C = (e-c).dot(e-c)-r*r;
    
    float disc = B*B - 4*A*C;
    
    if (disc <= 0) {
        //printf("no intersection\n");
        return 0;
    }
    
    disc = sqrt(disc);
    float pt1 = -(d.dot(e-c));
    float denom = A;
    
    float t1 = (pt1 + disc)/denom;
    float t2 = (pt1 - disc)/denom;
    
//    if (t1 < ray.t_min && t2 < ray.t_min && t1 > ray.t_max && t2 > ray.t_max) { return 0; }
    if(t1 < ray.t_min && t2 < ray.t_min){
        //printf("t1=%f\tt2=%f\tmin=%f\tmax=%f\n", t1, t2, ray.t_min, ray.t_max);
        return 0;
    }
    if(t1 > ray.t_max && t2 > ray.t_max){
        //printf("t1=%f\tt2=%f\tmin=%f\tmax=%f\n", t1, t2, ray.t_min, ray.t_max);
        return 0;
    }

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

    a = x;
    b = y;
    c = z; 
    
    normal = ((b-a).cross(c-a)).normalize();
    
}

bool Triangle::intersect(Ray ray, float &t_hit, LocalGeo &local) {
    float a2, b2, c2, d2, e2, f, g, h, i, j, k, l;
    
    Point e = ray.position;
    Vector d = ray.direction;
    
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
    
    Vector m_beta = Vector(e2*i - h*f, g*f - d2*i, d2*h - e2*g);
    Vector t_gamma = Vector(a2*k - j*b2, j*c2 - a2*l, b2*l - k*c2);
    
    Vector beta_t = Vector(j, k, l);
    Vector gamma_t = Vector(i, h, g);
    Vector t_t = Vector(f, e2, d2);
    Vector M_t = Vector(a2, b2, c2);
    
    float M = M_t.dot(m_beta);
    
    float t = -(t_t.dot(t_gamma))/M;
    if (t < ray.t_min || t > ray.t_max) { return 0; }
    
    float gamma = (gamma_t.dot(t_gamma))/M;
    if (gamma < 0 || gamma > 1) { return 0; }
    
    float beta = (beta_t.dot(m_beta))/M;
    if (beta < 0 || beta > 1 - gamma) { return 0; }
    
    Point temp = Point(0, 0, 0);
    /*
    Vector pos = (a-temp) + (b-a)*beta + (c-a)*gamma;
    Point position = Point(pos.x, pos.y, pos.z);*/
    Point position = e + d*t;
    Vector n = normal;
    /*
    printf("position ");
    position.print();
    printf("normal ");
    n.print();
    */
    local = LocalGeo(position, n);
    
    
    return 1;
}
