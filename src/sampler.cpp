#include "sampler.h"
#include "primitives.h"
#include "film.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

class Vector;

CameraFrame::CameraFrame(CameraSpec c) {
    
    Vector adj_v = -(c.lookAt - c.lookFrom);
    
    x = (c.up.cross(adj_v)).normalize();
    y = c.up.normalize();
    z = adj_v.normalize();

}

ScreenCoord::ScreenCoord (CameraSpec c, Film film, CameraFrame f) {
    
    double theta = c.fov/2;
    
    float adj = (c.lookAt - c.lookFrom).magnitude();
    float halfHeight = tan(theta)*adj;
    float height = halfHeight*2;
    
    float halfWidth = halfHeight*film.width*film.height;
    float width = halfWidth*2;
    
    LL = -f.x*halfWidth - f.y*halfHeight + f.z;
    UL = -f.x*halfWidth + f.y*halfHeight + f.z;
    LR = f.x*halfWidth - f.y*halfHeight + f.z;
    UR = f.x*halfWidth + f.y*halfHeight + f.z;
    
    step_x = width/film.width;
    step_y = height/film.height;
    
}

Sampler::Sampler() {
}

Sampler::Sampler(Film film) {
    
    width = film.width;
    height = film.height;
    i = 0;
    j = 0;
    
}

void Sampler::increment() {
    
    i++;
    if (i > width) {
        i = 0;
        j++;
    }
    
}

Sample::Sample(float u, float v) {

    x = u;
    y = v;
    
}

bool getSample(Sample* sample, Sampler s, ScreenCoord sc) {
    if (s.j > s.height) {
        
        return 0;
        
    } else {

        float x = sc.LL.x + (s.i+0.5)*sc.step_x;
        float y = sc.LL.y + (s.j+0.5)*sc.step_y;
        
        if (x < sc.LL.x || x > sc.UR.x || y < sc.LL.y || y > sc.UR.y) { 
            printf("Sample value out of range");
            exit (EXIT_FAILURE);
        }
    
        *sample = Sample(x, y);
    
        s.increment();
        
        return 1;
    }
}
