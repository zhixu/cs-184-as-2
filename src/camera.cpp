#include <stdio.h>
#include <stdlib.h>

#include "camera.h"

Camera::Camera (CameraSpec& cs) {
    spec = cs;
}

void Camera::generateRay (Sample& sample, Ray* ray) {
    float u, v;
    u = sample.x;
    v = sample.y;

    //ray->position = -(spec.z);
    //ray->direction = spec.lookFrom + spec.x*u + spec.y*v;
    
    
    ray->position = spec.lookFrom;
    ray->direction = (spec.LL * v + spec.UL * (1.0 - v)) * u
                   + (spec.LR * v + spec.UR * (1.0 - v)) * (1.0 - u);
                   
    printf("u and v %f %f\t", u, v);
    printf("ray direction %f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
    // TODO: check whether t values are correct
    ray->t_min = 0;
    ray->t_max = 1.0;
}


int main (int argc, char* argv[]) {
    
    generateRay();
    
}
