#include "sampler.h"
#include <math.h>

ScreenCoord ScreenCoord::ScreenCoord (CameraSpec c) { //ur gonna have to pass in the pointer for this one
    
    float theta = fov/2;
    float adj = magnitude(c.lookAt - c.lookFrom);
    float halfHeight = tan((double) theta)*adj;
    
    Vector basis_y = normalize(c.up);
    
    //field of view in the x direction will be determined by the image size
    float halfWidth = halfHeight*
    
}


bool getSample(Sample* sample);

    }
