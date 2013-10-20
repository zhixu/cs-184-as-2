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

    Vector d = lookFrom - lookAt;
    Vector w, u, v;
    w = d.normalize(); // points from camera to image plane
    u = w.cross(up).normalize(); // points right
    v = u.cross(w).normalize();

    double theta = fov/2;
    float distanceToImagePlane = sqrt(d.dot(d));
    float halfHeight = tan(theta * 3.141592653 /180.0) * distanceToImagePlane;
    float halfWidth = (halfHeight*film.width)/film.height;

    float pixelHeight, pixelWidth;
    pixelHeight = (halfHeight *2) / height;
    pixelWidth = (halfWidth * 2) / width;

    printf("distanceToImagePlane: %f\n", distanceToImagePlane);
    printf("halfHeight: %f\n", halfHeight);
    printf("halfWidth: %f\n", halfWidth);

    Vector center;
    center = d;
    ul = center + v * halfHeight - u * halfWidth;
    ur = center + v * halfHeight + u * halfWidth;
    ll = center - v * halfHeight - u * halfWidth;
    lr = center - v * halfHeight + u * halfWidth;
}


Point Sample::getSample(int x, int y) {
    float u = (width - x+0.5)/width;
    float v = (y+0.5 - height)/height;

    printf("x=%d, y=%d\tu=%f, v=%f\n", x, y, u, v);
    Vector temp = (ll*v + ul*(1-v))*u +
        (lr*v + ur*(1-v))*(1-u);
    Point p = Point(temp.x, temp.y, temp.z);
    return p;
}
