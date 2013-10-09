#ifndef SAMPLER_H
#define SAMPLER_H

#include <film.h>

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

class Sample {
    public:
        float x, y;
        Sample() { }
        Sample(CameraSpec, CameraFrame, Film);
        Sample(float, float);
        bool getSample(Sample);
};

//LL, UL, LR, UR are points given in WORLD SPACE
class ScreenCoord {
    public:
        float step_x, step_y;
        Vector LL, UL, LR, UR;
        ScreenCoord();
        ScreenCoord(CameraSpec, Film);
};

#endif
