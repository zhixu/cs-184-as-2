#include "primitives.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Vector class member functions
 */
 
Vector::Vector (float u, float v, float w) {
    
    x = u;
    y = v;
    z = w;
    a = 0; //signifies direction
    
}

Vector Vector::operator- () {
    Vector v;
    v.x = -x;
    v.y = -y;
    v.z = -z;
    
    return v;
    
}

Vector Vector::operator+ (Vector v) {
    Vector vector;
    
    vector.x = x + v.x;
    vector.y = y + v.y;
    vector.z = z + v.z;
    
    return vector;
}

Vector Vector::operator- (Vector v) {
    Vector vector;
    
    vector.x = x - v.x;
    vector.y = y - v.y;
    vector.z = z - v.z;
    
    return vector;
}

Vector Vector::operator* (float i) {
    Vector vector;
    
    vector.x = x*i;
    vector.y = y*i;
    vector.z = z*i;
    
    return vector;
}

Vector Vector::operator/ (float i) {
    Vector vector;
    
    vector.x = x/i;
    vector.y = y/i;
    vector.z = z/i;
    
    return vector;
}

Vector Vector::normalize (Vector v) {
    Vector vector;
    
    float distance = sqrt(x*x + y*y + z*z);
    
    vector.x = x/distance;
    vector.y = y/distance;
    vector.z = z/distance;

    return vector;
}

Vector Vector::magnitude(Vector v) {
    Vector w;
    
    w.x = abs(v.x);
    w.y = abs(v.y);
    w.z = abs(w.z);
    
    return w;
}

float Vector::dot(Vector u, Vector v) {
    float w;
    
    w = u.x * v.x + u.y * v.y + u.z * v.z;
    
    return w;
}

Vector Vector::cross(Vector a, Vector b) {
    Vector w;
    
    w.x = a.y*b.z - a.z*b.y;
    w.y = a.z*b.x - a.x*b.z;
    w.z = a.x*b.y - a.y*b.z;
        
    return w;
}

/*
 * Point class member functions
 */
Point::Point (float u, float v, float w) {
    
    x = u;
    y = v;
    z = w;
    a = 1; //signifies point
    
}

Point Point::operator+ (Vector v) {
    Point p;
    
    p.x = x + v.x;
    p.y = y + v.y;
    p.z = y + v.z;
    
    return p;
}

Point Point::operator- (Vector v) {
    Point p;
    
    p.x = x - v.x;
    p.y = y - v.y;
    p.z = y - v.z;
    
    return p;
}

Vector Point::operator- (Point p2) {
    float u, v, w;
    
    u = x - p2.x;
    v = y - p2.y;
    w = z - p2.z;
    
    return Vector(u, v, w);;
}

/*
 * Ray class member functions
 */
 
Ray::Ray (Point p, Vector v, float min, float max) {
    
    position = p;
    direction = v;
    t_min = min;
    t_max = max;
    
 }
 
 /*
 * Ray class member functions
 */
 
 Matrix Matrix::rotate() {
 }
 
 Matrix Matrix::scale() {
 }
 
 Matrix Matrix::translate() {
 }
 
 Matrix Matrix::invrotate() {
 }
 
 Matrix Matrix::invscale() {
 }
 
 Matrix Matrix::invtranslate() {
 }
 
 

/*
 * Transformation class member functions
 */

Point Transformation::operator* (Point& p) {
    float x, y, z;

    x = m.mat[0][0] * p.x + m.mat[0][1] * p.y + m.mat[0][2] * p.z;
    y = m.mat[1][0] * p.x + m.mat[1][1] * p.y + m.mat[1][2] * p.z;
    z = m.mat[2][0] * p.x + m.mat[2][1] * p.y + m.mat[2][2] * p.z;

    return Point(x, y, z);
}

Vector Transformation::operator* (Vector& v) {
    float x, y, z;

    x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z;
    y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z;
    z = m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z;

    return Vector(x, y, z);
}

Ray Transformation::operator* (Ray& r) {
    Ray result;

    result.position = this->operator*(r.position);
    result.direction = this->operator*(r.direction);
    // TODO: do we need to transform t_min and t_max when we transform a Ray?
    result.t_min = r.t_min;
    result.t_max = r.t_max;

    return result;
}

LocalGeo Transformation::operator* (LocalGeo& lg) {
    LocalGeo result;

    result.position = this->operator*(lg.position);
    //result.normal = this->operator*(lg.normal);

    return result;
}
/*
Normal Transformation::operator* (Normal& n){
    float x, y, z;

    x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z;
    y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z;
    z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z;

    return Normal(x, y, z);
}
*/

/*
 * Color class member functions
 */

Color::Color(){
    r = 0;
    g = 0;
    b = 0;
}

Color::Color (float paramR, float paramG, float paramB) {
    r = paramR;
    g = paramG;
    b = paramB;
}

Color Color::operator+ (Color& that) {
    Color* result = new Color(0, 0, 0);

    result->r = fmin(r + that.r, 255.0);
    result->g = fmin(g + that.g, 255.0);
    result->b = fmin(b + that.b, 255.0);

    return *result;
}

Color Color::operator- (Color& that) {
    Color* result = new Color(0, 0, 0);

    result->r = fmax(r - that.r, 0.0);
    result->g = fmax(g - that.g, 0.0);
    result->b = fmax(b - that.b, 0.0);

    return *result;
}

Color Color::operator* (float x) {
    Color result;

    result.r = fmin(255.0, r * x);
    result.g = fmin(255.0, g * x);
    result.b = fmin(255.0, b * x);

    return result;
}


