#ifndef CAMERA_H
#define CAMERA_H

#include "primitives.h"
#include "sampler.h"

class Camera {
    public:
        CameraSpec spec;
        Camera (CameraSpec&);
        void generateRay (Sample&, Ray*);
}

#endif
