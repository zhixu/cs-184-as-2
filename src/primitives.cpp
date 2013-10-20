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
                
    Vector w = Vector(y*b.z - z*b.y,
                        z*b.x-x*b.z,
                        x*b.y-y*b.x);
    
    return w;
}

float Vector::dot(Vector v) {
    float w;
    
    w = x * v.x + y * v.y + z * v.z;
    
    return w;
}

void Vector::print(){
    printf("Vector(%f, %f, %f)\n", x, y, z);
}

float Vector::magnitude(){
    return sqrt(x*x + y*y + z*z);
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

void Point::print(){
    printf("Point(%f, %f, %f)\n", x, y, z);
}

/*
 * Ray class member functions
 */

Ray::Ray(){
}

Ray::Ray (Point p, Vector v, float min, float max) {
    
    position = p;
    direction = v;
    t_min = min;
    t_max = max;
    
 }

void Ray::print(){
    printf("Ray(position=(%f, %f, %f), direction=(%f, %f, %f), t_min=%f, t_max=%f)\n",
            position.x, position.y, position.z,
            direction.x, direction.y, direction.z,
            t_min,
            t_max);
}
 
 /*
 * Matrix class member functions
 */
 
 Matrix::Matrix() {
     for(int i = 0; i < 4; i++) {
         for(int j = 0; j < 4; j++) {
             mat[i][j] = 0;
         }
     }
 }

Matrix::Matrix(const Matrix& orig){
    for(int r=0;r<4;r++){
        for(int c=0;c<4;c++){
            mat[r][c] = orig.mat[r][c];
        }
    }
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

void Matrix::identity(){
    for(int row=0; row<4; row++){
        for(int col=0; col<4; col++){
            if(row == col){
                mat[row][col] = 1;
            } else {
                mat[row][col] = 0;
            }
        }
    }
}

 // how to rotation matrix
 void Matrix::rotate(float x, float y, float z, float angle) {
    // using formulas from:
    // http://www.cs.princeton.edu/~gewang/projects/darth/stuff/quat_faq.html#Q54
    //
    // note that we negate the angle
    angle = -1.0 * angle;
    angle = angle * 3.141592653 / 180.0;
    
    Vector axis = Vector(x, y, z).normalize();
    float sin_a = sin(angle/2.0);
    float cos_a = cos(angle/2.0);

    // the quaternion
    float X = axis.x * sin_a;
    float Y = axis.y * sin_a;
    float Z = axis.z * sin_a;
    float W = cos_a;

    // the matrix values
    float xx, xy, xz, xw, yy, yz, yw, zz, zw;
    xx = X * X;
    xy = X * Y;
    xz = X * Z;
    xw = X * W;

    yy = Y * Y;
    yz = Y * Z;
    yw = Y * W;

    zz = Z * Z;
    zw = Z * W;

    mat[0][0] = 1 - 2 * (yy+zz);
    mat[0][1] =     2 * (xy+zw);
    mat[0][2] =     2 * (xz-yw);
    mat[0][3] = 0;

    mat[1][0] =     2 * (xy-zw);
    mat[1][1] = 1 - 2 * (xx+zz);
    mat[1][2] =     2 * (yz+xw);
    mat[1][3] = 0;

    mat[2][0] =     2 * (xz+yw);
    mat[2][1] =     2 * (yz-xw);
    mat[2][2] = 1 - 2 * (xx+yy);
    mat[2][3] = 0;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;
 }
 
void Matrix::scale(float x, float y, float z) {
    mat[0][0] = x;
    mat[1][1] = y;
    mat[2][2] = z;
    mat[3][3] = 1.0;
 }
 
void Matrix::translate(float x, float y, float z) {

    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;

    mat[0][0] = 1.0;
    mat[1][1] = 1.0;
    mat[2][2] = 1.0;
    mat[3][3] = 1.0;

 }

Matrix Matrix::transpose(){
    Matrix res = Matrix();
    for(int r=0;r<4;r++){
        for(int c=0;c<4;c++){
            res.mat[r][c] = mat[c][r];
        }
    }
    return res;
}
 
 Point Matrix::operator* (Point v) {
     Point u = Point();
     u.x = mat[0][0]*v.x + mat[0][1]*v.y + mat[0][2]*v.z + mat[0][3]*v.a;
     u.y = mat[1][0]*v.x + mat[1][1]*v.y + mat[1][2]*v.z + mat[1][3]*v.a;
     u.z = mat[2][0]*v.x + mat[2][1]*v.y + mat[2][2]*v.z + mat[2][3]*v.a;
     return u;
 }
 
  Vector Matrix::operator* (Vector v) {
     Vector u = Vector();
     u.x = mat[0][0]*v.x + mat[0][1]*v.y + mat[0][2]*v.z + mat[0][3]*v.a;
     u.y = mat[1][0]*v.x + mat[1][1]*v.y + mat[1][2]*v.z + mat[1][3]*v.a;
     u.z = mat[2][0]*v.x + mat[2][1]*v.y + mat[2][2]*v.z + mat[2][3]*v.a;
     return u;
 }

Matrix Matrix::operator* (Matrix o){
    Matrix result = Matrix();
    for(int row=0; row < 4; row++){
        for(int col=0; col < 4; col++){
            for(int k=0; k<4; k++){
                result.mat[row][col] += mat[row][k] * o.mat[k][col];
            }
        }
    }
    return result;
}

void Matrix::print(){
    for(int row=0; row<4; row++){
        printf("[%f, %f, %f, %f]\n", mat[row][0], mat[row][1], mat[row][2], mat[row][3]);
    }
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


Point Transformation::operator* (Point p) {
    float x, y, z;

    x = m.mat[0][0] * p.x + m.mat[0][1] * p.y + m.mat[0][2] * p.z;
    y = m.mat[1][0] * p.x + m.mat[1][1] * p.y + m.mat[1][2] * p.z;
    z = m.mat[2][0] * p.x + m.mat[2][1] * p.y + m.mat[2][2] * p.z;

    return Point(x, y, z);
}

Vector Transformation::operator* (Vector v) {
    float x, y, z;

    x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z;
    y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z;
    z = m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z;

    return Vector(x, y, z);
}

Ray Transformation::operator* (Ray r) {
    Ray result = Ray();

    result.position = m.operator*(r.position);
    result.direction = m.operator*(r.direction);
    // TODO: do we need to transform t_min and t_max when we transform a Ray?
    result.t_min = r.t_min;
    result.t_max = r.t_max;

    return result;
}

LocalGeo Transformation::operator* (LocalGeo lg) {
    LocalGeo result;

    result.position = (*this).operator*(lg.position);
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

Color Color::operator+ (Color that) {
    Color result = Color(0, 0, 0);

    result.r = fmin(fmax(r + that.r, 0), 1);
    result.g = fmin(fmax(g + that.g, 0), 1);
    result.b = fmin(fmax(b + that.b, 0), 1);

    return result;
}

Color Color::operator- (Color that) {
    Color result = Color(0, 0, 0);

    result.r = fmin(1, fmax(r - that.r, 0.0));
    result.g = fmin(1, fmax(g - that.g, 0.0));
    result.b = fmin(1, fmax(b - that.b, 0.0));

    return result;
}

Color Color::operator* (Color that) {
    
    Color result = Color(0, 0, 0);

    result.r = fmin(1, fmax(r * that.r, 0.0));
    result.g = fmin(1, fmax(g * that.g, 0.0));
    result.b = fmin(1, fmax(b * that.b, 0.0));

    return result;
    
}

Color Color::operator* (float x) {
    Color result;

    result.r = fmin(1, fmax(r * x, 0.0));
    result.g = fmin(1, fmax(g * x, 0.0));
    result.b = fmin(1, fmax(b * x, 0.0));

    return result;
}


Color Color::operator+= (Color that) {
    
    r = fmin(fmax(r + that.r, 0.0), 1);
    g = fmin(fmax(g + that.g, 0.0), 1);
    b = fmin(fmax(b + that.b, 0.0), 1);

    return *this;
}

Color Color::operator*= (float x) {
    
    r = fmin(fmax(r * x, 0), 1);
    g = fmin(fmax(g * x, 0), 1);
    b = fmin(fmax(b * x, 0), 1);

    return *this;
}

void Color::print(){
    printf("Color(%f, %f, %f)\n", r, g, b);
}

bool Color::isNotZero(){
    return (r > 0 && g > 0 && b > 0);
}

/*
 * Brdf class member functions
 */
 
 Brdf::Brdf() {
 }
 
 Brdf::Brdf(Color d, Color s, Color a, Color e, float shine) {
     
     kd = d;
     ks = s;
     ka = a;
     kr = s;
     emission = e;
     sp = shine;
 
 }

void Brdf::print(){
    printf("kd: ");
    kd.print();
    printf("ks: ");
    ks.print();
    printf("ka: ");
    ka.print();
    printf("kr: ");
    kr.print();
    printf("emission: ");
    emission.print();
}

 
 /*
 * Light class member functions
 */

Light::Light() {
}

/*Light::Light(Point p, Color c) {
    color = c;
    position = p;
}

void Light::generateShadowRay(LocalGeo local, Ray lightRay, Color lightColor){
    // do nothing because we don't know if this is point or directional light
    //
    // also, make sure t_min isn't 0, or else we'll think the ShadowRay
    // intersects the source of the original intersection!
    printf("generating nothing fuckdamnit\n");
}*/

PointLight::PointLight() {
}

Vector PointLight::getLm(Point p) {
    
    return Vector(position.x - p.x,
                                  position.y - p.y,
                                  position.z - p.z);
    
}

void PointLight::generateShadowRay(LocalGeo local, Ray &shadowRay, Color lightColor){
    Vector vector = position - local.position;
    /*printf("DIRECTION OF SHADOW RAY\t");
    vector.print();
    printf("POSITION OF SHADOW RAY\t");
    local.position.print();*/
    
    shadowRay = Ray(local.position, vector, 0.0001, 900);
    lightColor = color;
}

DirectionalLight::DirectionalLight() {
}

Vector DirectionalLight::getLm(Point p) {
    return Vector(position.x, position.y, position.z);
}

void DirectionalLight::generateShadowRay(LocalGeo local, Ray &shadowRay, Color lightColor){
    Vector vector = Vector(position.x,
                                position.y,
                                position.z);
    /*
    printf("DIRECTION OF SHADOW RAY\t");
    vector.print();
    printf("POSITION OF SHADOW RAY\t");
    local.position.print();*/
    
    shadowRay = Ray(local.position, vector, 0.0001, 900);
    lightColor = color;
}
