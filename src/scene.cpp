#include <stdio.h>
#include <stdlib.h>

#include "sampler.h"
#include "film.h"
#include "shapes.h"


/*

void render(Sampler sampler, Sample sample, Film film, CameraSpec cs, Sphere sphere, ) {
    
        while (sampler.getSample(sampler, sample, sc)) {
            
            camera.generateRay(sample, 1, &ray);
            raytracer.trace(ray, &color);
            film.commit(sample, color);
            }
            film.write((char*) "test.png");
}
*/

int main (int argc, char* argv[]) {
    // creates all the needed variables
    // pass variables by reference into parser
    // parser adds necessary information
    // calls appropriate functions to generate result
    
    //in lieu of parser
    Film film = Film(100, 100);
    
    Point lookFrom = Point(0.0, 0.0, 0.0);
    Point lookAt = Point (0.0, 0.0, -1.0);
    Vector up = Vector(0.0, 1.0, 0.0);
    float angle = 90.0;
    Sample sample = Sample(lookFrom, lookAt, up, angle, film);
    
    Point center = Point(0.0, 0.0, -5.0);
    float radius = 1.0;
    Sphere sphere = Sphere(center, radius);
    
    

    Color* color = new Color(255, 0, 0);
    
    for(int i = 0; i < film.height; i++) {
        for(int j = 0; j < film.width; j++) {
         
            Vector direction = sample.getSample(j, i) - lookFrom;
            Ray r = Ray(lookFrom, direction, 0, -5);
        
            //printf("direction: %f %f %f   coordinates: x %d y %d\n", direction.x, direction.y, direction.z, j, i);
            if (sphere.intersectP(r)) { film.commit(i, j, color); }
        }
    }
        
    film.write((char *) "test.png");
    //render(sampler, sample, film, cs, sphere);
    
}
