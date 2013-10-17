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
        Vector normalize();
        Vector cross(Vector);
        float dot(Vector);
        void print();
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
        Point operator* (float);
        void print();
};

class Ray {
    public: 
        Point position;
        Vector direction;
        float t_min, t_max;
        Ray();
        Ray(Point, Vector, float, float);
        void print();
};

class Matrix {
    public:
        float mat[4][4];
        Matrix();
        Matrix(float, float, float, float,
                float, float, float, float, 
                float, float, float, float, 
                float, float, float, float);
        void rotate(float, float, float);
        void scale(float, float, float);
        void translate(float, float, float);
        Matrix invrotate(Matrix);
        Matrix invscale(Matrix);
        Matrix invtranslate(Matrix);
        Point operator* (Point);
        Vector operator* (Vector);
        
};

class LocalGeo {
    public:
        Point position;
        Vector normal;
    LocalGeo();
    LocalGeo(Point, Vector);
};

class Transformation {
    private:
        Matrix m, inverseTransposeM;

    public:
        // TODO: decide arguments for constructor
        Point operator* (Point);
        Vector operator* (Vector);
        Ray operator* (Ray);
        LocalGeo operator* (LocalGeo);
        //Normal operator* (Normal);
};

class Color {
    public:
        float r, g, b;
        Color();
        Color (float, float ,float);
        Color operator+ (Color);
        Color operator- (Color);
        Color operator* (Color);
        Color operator* (float);
        Color operator/ (float);
        Color operator+= (Color);
        Color operator*= (float);
        void print();
        bool isNotZero();
};

class Brdf {
    public:
        Color kd, ks, ka, emission, kr;
        float sp;
        Brdf();
        Brdf(Color, Color, Color, Color, float);
        void print();
};

class Light {
    public:
        Color color;
        Point position;
        Light();
        Light(Point, Color);
        virtual void generateShadowRay(LocalGeo, Ray, Color);
        virtual Vector getLm(Point) = 0;
};

class PointLight : public Light {
    public:
        PointLight(Point, Color);
        Vector getLm(Point);
        void generateShadowRay(LocalGeo, Ray&, Color);
};

class DirectionalLight : public Light {
    public:
        DirectionalLight(Point, Color);
        Vector getLm(Point);
        void generateShadowRay(LocalGeo, Ray&, Color);
};


#endif /* PRIMITIVES_H */
