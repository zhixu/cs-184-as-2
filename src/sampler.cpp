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

    
    Vector d = lookAt-lookFrom;
    
    //ORTHONORMAL BASIS FOR CAMERA
/*    x = (d.cross(up)).normalize();
    y = up.normalize();
    z = d.normalize(); //camera looks in -z direction*/
    
    z = d.normalize();
    x = (up.cross(z)).normalize();
    y = (z.cross(x)).normalize();
    
    double theta = fov/2;
    float dd = d.dot(d);
    float halfHeight = tan(theta * 3.141592653 /180.0)*sqrt(dd);
    float halfWidth = (halfHeight*film.width)/film.height;
    
    //BASIS FOR IMAGE PLANE
    u = x*halfWidth;
    v = y*halfHeight;

}


Point Sample::getSample(int x, int y) {
    
    
    float alpha = 2*(x + 0.5)/width -1;
    float beta = 1 - 2*(y+0.5)/height;
    
    Vector d = u*alpha + v*beta - z;
    
    Point p = Point(d.x, d.y, d.z);
    
    return p;
    
}
