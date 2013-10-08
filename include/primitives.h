class Vector {
    public:
        float x, y, z, a;
        Vector(float, float, float);
        Vector operator+ (Vector);
        Vector operator- (Vector);
        Vector operator* (float);
        Vector operator/ (float);
        Vector normalize(Vector);
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
        Point(float, float, float);
        Point operator+ (Vector); //point + vector(displacement) = point (displaced)
        Point operator- (Vector); //point - vector = point
        Vector operator- (Point); //point - point = vector
        
};

class Ray {
        Point pos;
        Vector dir;
        float t_min, t_max;
    public: 
        Ray(point, vector, float, float); //ray constructor?
};

class Matrix {
        float mat[4][4];
    public:
        matrix rotate();
        matrix scale();
        matrix translate();
        matrix invrotate();
        matrix invscale();
        matrix invtranslate();
        
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
        Normal operator* (Normal);
};

class Color {
    public:
        float r, g, b;

        Color (float, float ,float);
        Color operator+ (Color);
        Color operator- (Color);
        Color operator* (float);
        Color operator/ (float);
};

class Brdf {
    public:
        Color kd, ks, ka, kr;
};

class Sample {
    public:
        float x, y;
};

class LocalGeo {
    public:
        Point position;
        Normal normal;
};
