#include "camera.h"

Camera::Camera (CameraSpec& cs) {
    spec = cs;
}

void Camera::generateRay (Sample& sample, Ray* ray) {
    float u, v;
    u = sample->x;
    v = sample->y;

    ray->position = spec->lookFrom;

    ray->direction = u * (v * spec.LL + (1.0 - v) * spec.UL)
                   + (1.0 - u) * (v * spec.LR + (1.0 - v) * spec.UR);
    // TODO: check whether t values are correct
    ray->t_min = 0;
    ray->t_max = 1.0;
}
