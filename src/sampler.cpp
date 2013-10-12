#include "sampler.h"
#include "primitives.h"
//#include "film.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

//class Vector;

Sample::Sample() {
}

Sample::Sample(Point lf, Point la, Vector upv, float angle, Film film) {

    lookFrom = lf;
    lookAt = la;
    up = upv;
    fov = angle;
    
    width = film.width;
    height = film.height;

    
    Vector d = lookFrom - lookAt; //camera looks in -z direction
    
    //ORTHONORMAL BASIS FOR CAMERA
    x = (d.cross(up)).normalize();
    y = up.normalize();
    z = d.normalize(); //camera looks in -z direction
    
    /*
    printf("basis x: %f %f %f\n", x.x, x.y, x.z);
    printf("basis y: %f %f %f\n", y.x, y.y, y.z);
    printf("basis z: %f %f %f\n", z.x, z.y, z.z);
    */
    
    double theta = fov/2;
    float dd = d.dot(d);
    float halfHeight = tan(theta/180)*dd;
    float halfWidth = (halfHeight*film.width)/film.height;
    
    //BASIS FOR IMAGE PLANE
    u = x*halfWidth;
    v = y*halfHeight;
    
    /*
    printf("halfwidth: %f      halfheight %f\n", halfWidth, halfHeight);
    printf("basis u: %f %f %f\n", u.x, u.y, u.z);
    printf("basis v: %f %f %f\n", v.x, v.y, v.z);
        
    printf("width: %d height: %d\n", width, height);
    */
}


Point Sample::getSample(int x, int y) {
    
    
    float alpha = 2*(x + 0.5)/width -1;
    float beta = 1 - 2*(y+0.5)/height;
    
    //printf("alpha: %f beta: %f          x: %d  y: %d\n", alpha, beta, r, s);
    
    Vector d = u*alpha + v*beta + z;
    
    Point p = Point(d.x, d.y, d.z);
    
    return p;
    
}
