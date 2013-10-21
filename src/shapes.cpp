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
    
    Point e = worldToObject * ray.position;
    Vector d = (worldToObject * ray.direction);
    
    float disc = pow(d.dot(e-c), 2) - d.dot(d)*((e-c).dot(e-c) - r*r);
    
    if (disc < 0) { return 0; }
    
    disc = sqrt(disc);
    float pt1 = -(d.dot(e-c));
    float denom = d.dot(d);
    
    float t1 = (pt1 + disc)/denom;
    float t2 = (pt1 - disc)/denom;
    
    if(t1 < ray.t_min && t2 < ray.t_min){
        //printf("t1=%f\tt2=%f\tmin=%f\tmax=%f\n", t1, t2, ray.t_min, ray.t_max);
        //printf("Hit but lower than t_min");
        return 0;
    }
   if(t1 > ray.t_max && t2 > ray.t_max){
        //printf("t1=%f\t t2=%f\t min=%f\t max=%f\n", t1, t2, ray.t_min, ray.t_max);
        //printf("Hit but higher than t_max");
        return 0;
    }
/*
   if(! ((t1 > ray.t_min && t1 < ray.t_max) || (t2 > ray.t_min && t2 < ray.t_max))){
        return 0;
   }
*/
    Point p1 = e + d*t1;
    Point p2 = e + d*t2;
    
    float distance1 = (p1-e).dot(p1-e);
    float distance2 = (p2-e).dot(p2-e);
    
    Point position;
    Vector normal;
    if (distance1 < distance2) {
     //   if (t1 < ray.t_min || t1 > ray.t_max) { return 0; }
        thit = t1;
        position = p1;
        normal = (p1-c);
    } else {
       // if (t2 < ray.t_min || t2 > ray.t_max) { return 0; }
        thit = t2;
        position = p2;
        normal = (p2-c);
    }
    
    local = LocalGeo(objectToWorld * position, (worldToObject.transpose() * normal).normalize());
    
    return 1;
}

Triangle::Triangle() {
    
}

Triangle::Triangle(Point x, Point y, Point z) {

    a = x;
    b = y;
    c = z; 
    
    normal1 = ((c-a).cross(b-a)).normalize();
    normal2 = ((b-a).cross(c-a)).normalize();
    
}

bool Triangle::intersect(Ray ray, float &t_hit, LocalGeo &local) {
    float a2, b2, c2, d2, e2, f, g, h, i, j, k, l;
    
    Point e = worldToObject * ray.position;
    Vector d = worldToObject * ray.direction;
    
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
    
    Point position = e + d*t;
    t_hit = t;
    
    if (d.dot(normal1) <= 0) {
        normal = normal1;
    } else if (d.dot(normal2) < 0) {
        normal = normal2;
    }
    
    local = LocalGeo(objectToWorld * position, (worldToObject.transpose() * normal).normalize());
    
    return 1;
}
