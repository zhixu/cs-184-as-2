#ifndef SAMPLER_H
#define SAMPLER_H

#include "primitives.h"
#include "film.h"

class Film; // need this so that g++ doesn't get confused

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

//LL, UL, LR, UR are points given in WORLD SPACE
class ScreenCoord {
    public:
        float step_x, step_y;
        Vector LL, UL, LR, UR;
        ScreenCoord();
        ScreenCoord(CameraSpec, Film, CameraFrame);
};

class Sampler {
  
  public:
        int i, j;
        int width, height;
        Sampler();
        Sampler(Film film);
        void increment();
    
};

class Sample {
    public:
        float x, y;
        Sample();
        Sample(float, float);
        bool getSample(Sample);
};

#endif
