class Sample {
    public:
        float x, y;
        get
} sample;

class CameraSpec {
    public:
        Point lookFrom, lookAt;
        Vector up;
        float fov;
}

class ScreenCoord {
    public:
        float UL, LL, UR, LL;
        ScreenCoord(CameraSpec);
} screencoord;
