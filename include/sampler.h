#include <film.h>

#ifndef SAMPLER_H
#define SAMPLER_H


class Sample {
    public:
        float x, y;
        Sample();
        Sample(CameraFrame, Film);
        Sample(float, float);
        bool getSample(Sample);
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
    CameraFrame();
    CameraFrame(CameraSpec);
};

class ScreenCoord {
    public:
        float step_x, step_y;
        Vector LL, UL, LR, UR;
        ScreenCoord();
        ScreenCoord(CameraSpec, Film);
};

#endif
