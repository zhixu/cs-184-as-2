#include <stdio.h>
#include <stdlib.h>

#include "sampler.h"
#include "camera.h"
#include "film.h"




void render() {
        while (!sampler.generateSample(&sample) {
            camera.generateRay(sample, &ray);
            raytracer.trace(ray, &color);
            film.commit(sample, color);
            }
            film.writeImage();
}


int main (int argc, char* argv[]) {
    
    render();
    
}
