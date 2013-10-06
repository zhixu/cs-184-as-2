#include "primitives.h"
#include <math.h>

/*
 * Transformation class member functions
 */

Point Transformation::operator* (Point p) {
    float x, y, z;

    x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z;
    y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z;
    z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z;

    return Point(x, y, z);
}

Vector Transformation::operator* (Vector v) {
    float x, y, z;

    x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z;
    y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z;
    z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z;

    return Vector(x, y, z);
}

Ray Transformation::operator* (Ray r) {
    Ray result;

    result.position = this * r.position;
    result.direction = this * r.direction;
    // TODO: do we need to transform t_min and t_max when we transform a Ray?
    result.t_min = r.t_min;
    result.t_max = r.t_max;

    return result;
}

LocalGeo Transformation::operator* (LocalGeo lg) {
    LocalGeo result;

    result.position = this * lg.position;
    result.normal = this * lg.normal;

    return result;
}

Normal Transformation::operator* (Normal n){
    float x, y, z;

    x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z;
    y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z;
    z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z;

    return Normal(x, y, z);
}


/*
 * Color class member functions
 */

Color::Color (float paramR, float paramG, float paramB) {
    r = paramR;
    g = paramG;
    b = paramB;
}

Color Color::operator+ (Color that) {
    Color result;

    result.r = fmin(r + that.r, 255.0);
    result.g = fmin(g + that.g, 255.0);
    result.b = fmin(b + that.b, 255.0);

    return result;
}

Color Color::operator- (Color that) {
    Color result;

    result.r = fmax(r - that.r, 0.0);
    result.g = fmax(g - that.g, 0.0);
    result.b = fmax(b - that.b, 0.0);

    return result;
}

Color Color::operator* (float x) {
    Color result;

    result.r = fmin(255.0, r * x);
    result.g = fmin(255.0, g * x);
    result.b = fmin(255.0, b * x);

    return result;
}


