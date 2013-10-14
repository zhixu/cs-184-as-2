#include "primitives.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Vector class member functions
 */

Vector::Vector(){
}

Vector::Vector (float u, float v, float w) {
    
    x = u;
    y = v;
    z = w;
    a = 0; //signifies direction
    
}

Vector Vector::operator- () {
    Vector v = Vector(-x, -y, -z);
    return v;
}

Vector Vector::operator+ (Vector v) {
    Vector vector = Vector(x + v.x,
                                y + v.y,
                                z + v.z);
    return vector;
}

Vector Vector::operator- (Vector v) {
    Vector vector = Vector(x - v.x,
                                y - v.y,
                                z - v.z);
    return vector;
}

Vector Vector::operator* (float i) {
    Vector vector = Vector(x * i, y * i, z * i);
    return vector;
}

Vector Vector::operator/ (float i) {
    Vector vector = Vector(x/i, y/i, z/i);
    return vector;
}

Vector Vector::normalize() {
    float distance = sqrt(x*x + y*y + z*z);
    Vector vector = Vector(x/distance, y/distance, z/distance);

    return vector;
}

Vector Vector::cross(Vector b) {
    Vector w = Vector(z*b.y - y*b.z,
                -(x*b.z - z*b.x),
                x*b.y - y*b.x);
    
    return w;
}

float Vector::dot(Vector v) {
    float w;
    
    w = x * v.x + y * v.y + z * v.z;
    
    return w;
}

/*
 * Point class member functions
 */
Point::Point(){
}

Point::Point (float u, float v, float w) {
    x = u;
    y = v;
    z = w;
    a = 1; //signifies point
}

Point Point::operator+ (Vector v) {
    Point p = Point(x + v.x,
                         y + v.y,
                         z + v.z);
    return p;
}

Point Point::operator- (Vector v) {
    Point p = Point(x - v.x,
                         y - v.y,
                         z - v.z);
    return p;
}

Vector Point::operator- (Point p2) {
    float u, v, w;
    u = x - p2.x;
    v = y - p2.y;
    w = z - p2.z;
    Vector ret = Vector(u, v, w);
    return ret;
}

/*
 * Ray class member functions
 */

Ray::Ray(){
}

Ray::Ray (Point& p, Vector& v, float min, float max) {
    
    position = p;
    direction = v;
    t_min = min;
    t_max = max;
    
 }
 
 /*
 * Matrix class member functions
 */
 
 Matrix::Matrix() {
 }
 
 Matrix::Matrix(float a1, float a2, float a3, float a4,
                float b1, float b2, float b3, float b4,
                float c1, float c2, float c3, float c4,
                float d1, float d2, float d3, float d4) {
                    
                    mat[0][0] = a1;
                    mat[0][1] = a2;
                    mat[0][2] = a3;
                    mat[0][3] = a4;
                    mat[1][0] = b1;
                    mat[1][1] = b2;
                    mat[1][2] = b3;
                    mat[1][3] = b4;
                    mat[2][0] = c1;
                    mat[2][1] = c2;
                    mat[2][2] = c3;
                    mat[2][3] = c4;
                    mat[3][0] = d1;
                    mat[3][1] = d2;
                    mat[3][2] = d3;
                    mat[3][3] = d4;

}
 
 
 Matrix Matrix::rotate(float x, float y, float z) {
 
    Matrix m = Matrix (x, 0.0, 0.0, 0.0,
                             0.0, y, 0.0, 0.0,
                             0.0, 0.0, z, 0.0,
                             0.0, 0.0, 0.0, 1.0);
    
    return m;
 
 }
 
 Matrix Matrix::scale(float x, float y, float z) {
     Matrix m = Matrix (x, 0.0, 0.0, 0.0,
                             0.0, y, 0.0, 0.0,
                             0.0, 0.0, z, 0.0,
                             0.0, 0.0, 0.0, 1.0);
     
     return m;
 }
 
 Matrix Matrix::translate(float x, float y, float z) {
     Matrix m = Matrix (1.0, 0.0, 0.0, x,
                             0.0, 1.0, 0.0, y,
                             0.0, 0.0, 1.0, z,
                             0.0, 0.0, 0.0, 1.0);
                             
    return m;
 }
 
 Matrix Matrix::invrotate(Matrix m) {
     Matrix n = Matrix(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                            mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
     
     return n;
 }
 
 Matrix Matrix::invscale(Matrix m) {
     
     Matrix n = Matrix(1/mat[0][0], 1/mat[0][1], 1/mat[0][2], 1/mat[0][3],
                            1/mat[1][0], 1/mat[1][1], 1/mat[1][2], 1/mat[1][3],
                            1/mat[2][0], 1/mat[2][1], 1/mat[2][2], 1/mat[2][3],
                            1/mat[3][0], 1/mat[3][1], 1/mat[3][2], 1/mat[3][3]);
     return n;
 }
 
 Matrix Matrix::invtranslate(Matrix m) {
     
     Matrix n = Matrix(-mat[0][0], -mat[0][1], -mat[0][2], -mat[0][3],
                            -mat[1][0], -mat[1][1], -mat[1][2], -mat[1][3],
                            -mat[2][0], -mat[2][1], -mat[2][2], -mat[2][3],
                            -mat[3][0], -mat[3][1], -mat[3][2], -mat[3][3]);
     return n;
     
 }
 
/*
 * LocalGeo class member functions
 */
 
 LocalGeo::LocalGeo() {
 }
 
 LocalGeo::LocalGeo(Point p, Vector v) {
     
     position = p;
     normal = v;
     
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
    Ray* result = new Ray();

    result->position = this->operator*(r.position);
    result->direction = this->operator*(r.direction);
    // TODO: do we need to transform t_min and t_max when we transform a Ray?
    result->t_min = r.t_min;
    result->t_max = r.t_max;

    return *result;
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


Color& Color::operator+= (Color& that) {
    r = fmin(r + that.r, 255.0);
    g = fmin(g + that.g, 255.0);
    b = fmin(b + that.b, 255.0);

    return *this;
}

Color& Color::operator*= (float x) {
    r = fmin(r * x, 255.0);
    g = fmin(g * x, 255.0);
    b = fmin(b * x, 255.0);

    return *this;
}

/*
 * Brdf class member functions
 */
 
 Brdf::Brdf() {
 }
 
 Brdf::Brdf(Color d, Color s, Color a, float r) {
     
     kd = d;
     ks = s;
     ka = a;
     kr = r;
 
 }
 
 /*
 * Light class member functions
 */

Light::Light() {
}

Light::Light(Point* p, Color* c) {
    color = *c;
    position = *p;
}

void Light::generateShadowRay(LocalGeo* local, Ray* lightRay, Color* lightColor){
    // do nothing because we don't know if this is point or directional light
    //
    // also, make sure t_min isn't 0, or else we'll think the ShadowRay
    // intersects the source of the original intersection!
}

PointLight::PointLight(Point* p, Color* c) : Light(p, c) {
}

void PointLight::generateShadowRay(LocalGeo* local, Ray* shadowRay, Color* lightColor){
    Vector* vector = new Vector(position.x - local->position.x,
                                  position.y - local->position.y,
                                  position.z - local->position.z);

    shadowRay = new Ray(local->position, *vector, 0.01, 0);
    *lightColor = color;
}

DirectionalLight::DirectionalLight (Point* p, Color* c) : Light(p, c){
}

void DirectionalLight::generateShadowRay(LocalGeo* local, Ray* shadowRay, Color* lightColor){
    Vector* vector = new Vector(position.x,
                                position.y,
                                position.z);
    shadowRay = new Ray(local->position, *vector, 0.01, 0);
    *lightColor = color;
}
