class Sample {
    public:
        float x, y;
        Sample(CameraFrame, Film);
        Sample(float, float);
        getSample(Sample);
};

class CameraSpec {
    public:
        Point lookFrom, lookAt;
        Vector up;
        float fov;
};

//orthonormal basis for camera
class CameraFrame {
    public:
        Vector x, y, z;
    CameraFrame(Vector, Vector, Vector);
};

class ScreenCoord {
    public:
        float step_x, step_y;
        Vector LL, UL, LR, UR;
        ScreenCoord(CameraSpec);
};
