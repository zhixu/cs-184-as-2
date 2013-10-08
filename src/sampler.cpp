#include "sampler.h"
#include "primitives.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

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
    float adj = magnitude((Vector) c.lookAt - c.lookFrom);
    float halfHeight = tan(theta)*adj;
    float height = halfHeight*2;
    
    float halfWidth = halfHeight*film.width*film.height;
    float width = halfWidth*2;
    
    LL = -halfWidth*(cameraf.x) - halfHeight*(cameraf.y) + cameraf.z;
    LR = halfWidth*(cameraf.x) - halfHeight*(cameraf.y) + cameraf.z;
    UL = -halfWidth*(cameraf.x) + halfHeight*(cameraf.y) + cameraf.z;
    UR = halfWidth*(cameraf.x) + halfHeight*(cameraf.y) + cameraf.z;
    
    step_x = width/film.width;
    step_y = height/film.height;
    
}

Sample(CameraSpec c, Film film) {
    
    s = new ScreenCoord(c, film);
    cameraf = new CameraFrame(c);
    
    f = film;
    
}

Sample(float u, float v) {

    x = u;
    y = v;

    if (x < LL.x || x > UR.x || y < LL.y || y > UR.y) { 
        printf("Sample value out of range");
        exit (EXIT_FAILURE);
    }
}

bool getSample(Sample* sample) {
    if (j > f.height) {
        return 0;
    } else {

        x = LL.x + (i+0.5)*s.step_x;
        y = LL.y + (j+0.5)*s.step_y;
    
        sample = new Sample(x, y);
        
        i++;
        if (i > f.width) {
            i = 0;
            j++;
        }
        return 1;
    }
}
