#include "sampler.h"
#include "primitives.h"
//#include "film.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

//class Vector;

Sample::Sample() {
}

Sample::Sample(Point lf, Point la, Vector upv, float angle, Film film, int rpp) {
   
    lookFrom = lf;
    lookAt = la;
    up = upv;
    fov = angle;
    raysPerPixel = rpp;

    width = film.width;
    height = film.height;

    Vector d = lookFrom - lookAt;
    w = d.normalize(); // points from camera to image plane
    u = up.cross(w).normalize(); // points right
    v = w.cross(u).normalize();

    tanHalfY = tan((fov/2.0) * (3.141592653/180.0));
    tanHalfX = tanHalfY * ((float) width) / ((float) height);

    halfWidth = (float) width / 2.0;
    halfHeight = (float) height / 2.0;
}


Point Sample::getSample(int x, int y) {
    float alpha, beta;
    float offsetX, offsetY;
    if(raysPerPixel > 1){
        offsetX = (float) rand() / RAND_MAX;
        offsetY = (float) rand() / RAND_MAX;
    } else {
        offsetX = 0.5;
        offsetY = 0.5;
    }
    alpha = tanHalfX * ((x + offsetX - halfWidth)/halfWidth);
    beta = tanHalfY * ((halfHeight - (y + offsetY))/halfHeight);
    Vector temp = (u * alpha + v * beta - w);
    Point p = Point(temp.x, temp.y, temp.z);
    return p;
}
