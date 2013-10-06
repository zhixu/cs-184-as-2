
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
}

class Color {
    public:
        float r, g, b;

        Color (float, float ,float);
        Color operator+ (Color);
        Color operator- (Color);
        Color operator* (float);
        Color operator/ (float);
}

class Brdf {
    public:
        Color kd, ks, ka, kr;
}

class Sample {
    public:
        float x, y;
}

class LocalGeo {
    public:
        Point position;
        Normal normal;
}
