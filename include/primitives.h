#ifndef PRIMITIVES_H
#define PRIMITIVES_H


class Vector {
    public:
        float x, y, z, a;
        Vector();
        Vector(float, float, float);
        Vector operator- ();
        Vector operator+ (Vector);
        Vector operator- (Vector);
        Vector operator* (float);
        Vector operator/ (float);
        Vector normalize(Vector);
        Vector cross(Vector, Vector);
        float dot(Vector, Vector);
        Vector magnitude(Vector);
};

/*
 * 
 * Not sure if this class is actually needed--can just use vector?
 * any ideas why a normal class is important?
 * keeping this commented out for now.
 * 
 * 
class Normal { 
    
        int x, y, z, a;
    public:
        Normal(float, float, float);
        

} normal;
*/


class Point {
    public:
        float x, y, z, a;
        Point();
        Point(float, float, float);
        Point operator+ (Vector); //point + vector(displacement) = point (displaced)
        Point operator- (Vector); //point - vector = point
        Vector operator- (Point); //point - point = vector
        
};

class Ray {
    public: 
        Point position;
        Vector direction;
        float t_min, t_max;
        Ray();
        Ray(Point, Vector, float, float);
};


class Matrix {
    public:
        float mat[4][4];
        Matrix();
        Matrix rotate();
        Matrix scale();
        Matrix translate();
        Matrix invrotate();
        Matrix invscale();
        Matrix invtranslate();
        
};

class LocalGeo {
    public:
        Point position;
        //Normal normal;
};

class Transformation {
    private:
        Matrix m, inverseTransposeM;

    public:
        // TODO: decide arguments for constructor
        Point operator* (Point&);
        Vector operator* (Vector&);
        Ray operator* (Ray&);
        LocalGeo operator* (LocalGeo&);
        //Normal operator* (Normal);
};

class Color {
    public:
        float r, g, b;
        Color();
        Color (float, float ,float);
        Color operator+ (Color&);
        Color operator- (Color&);
        Color operator* (float);
        Color operator/ (float);
};

class Brdf {
    public:
        Color kd, ks, ka, kr;
};

#endif /* PRIMITIVES_H */
