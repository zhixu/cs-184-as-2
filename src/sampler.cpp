#include "sampler.h"
#include "primitives.h"
#include "film.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

class Vector;

int i, j; //counters for stepping
ScreenCoord s;
CameraFrame cameraf;
Film f;

CameraFrame::CameraFrame(CameraSpec c) {
    
    Vector adj_v = -(c.lookAt - c.lookFrom);
    
    x = normalize(cross(c.up, adj_v));
    y = normalize(c.up);
    z = normalize(adj_v);

}

ScreenCoord::ScreenCoord (CameraSpec c, Film film) {
    
    double theta = c.fov/2;
    float adj = magnitude(c.lookAt - c.lookFrom);
    float halfHeight = tan(theta)*adj;
    float height = halfHeight*2;
    
    float halfWidth = halfHeight*film.width*film.height;
    float width = halfWidth*2;
    
    LL = -cameraf.x*halfWidth - cameraf.y*halfHeight + cameraf.z;
    UL = -cameraf.x*halfWidth + cameraf.y*halfHeight + cameraf.z;
    LR = cameraf.x*halfWidth - cameraf.y*halfHeight + cameraf.z;
    UR = cameraf.x*halfWidth + cameraf.y*halfHeight + cameraf.z;
    
    step_x = width/film.width;
    step_y = height/film.height;
    
}

Sample::Sample(CameraSpec cs, CameraFrame c, Film film) {
    
    s = ScreenCoord(cs, film);
    cameraf = c;
    
    f = film;
    
}

Sample::Sample(float u, float v) {

    x = u;
    y = v;

    if (x < s.LL.x || x > s.UR.x || y < s.LL.y || y > s.UR.y) { 
        printf("Sample value out of range");
        exit (EXIT_FAILURE);
    }
}

bool getSample(Sample* sample) {
    if (j > f.height) {
        return 0;
    } else {

        float x = s.LL.x + (i+0.5)*s.step_x;
        float y = s.LL.y + (j+0.5)*s.step_y;
    
        sample = new Sample(x, y);
        
        i++;
        if (i > f.width) {
            i = 0;
            j++;
        }
        return 1;
    }
}
