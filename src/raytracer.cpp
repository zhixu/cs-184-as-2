#include <vector>
#include <math.h>
#include "raytracer.h"

#define DEPTH_THRESHOLD 1

RayTracer::RayTracer (std::vector< Shape* > ss, std::vector< Light* > ls) {
    shapes = ss;
    lights = ls;
}

void RayTracer::trace(Ray* ray, int depth, Color* color){
    if(depth > DEPTH_THRESHOLD){
        // Color's default constructor makes it black,
        // no need to do anything here
        return;
    }

    float t_of_hit;
    LocalGeo* intersection;
    Shape* shape;
    bool has_intersection = false;
    for(std::vector<int>::size_type i=0; has_intersection == false && i != shapes.size(); i++){
        shape = shapes[i];

        if(shape->intersect(ray, &t_of_hit, &intersection)){
            has_intersection = true;
            break;
        }
    }

    if(!has_intersection){
        return;
    }

    Brdf* brdf;
    shape->getBrdf(intersection, &brdf);

    Light* light;
    Ray* shadowRay;
    Color* lightColor;
    for(std::vector<int>::size_type i=0; i != lights.size(); i++){
        light = lights[i];
        light->generateShadowRay(intersection, shadowRay, lightColor);

        // check if the light ray is blocked
        bool intersectsOtherObject = false;
        // loop over each of the other objects and check if this shadow ray hits
        for(std::vector<int>::size_type j=0; intersectsOtherObject == false && j != shapes.size(); i++){
            shape = shapes[j];
            if(shape->intersect(shadowRay, &t_of_hit, &intersection)){
                intersectsOtherObject = true;
                break;
            }
        }

        if(!intersectsOtherObject){
            *color += shading(intersection, brdf, shadowRay, lightColor);
        }

        // TODO: handle reflection
    }
}

Color illuminate(Point lookAt, LocalGeo* local, Brdf* brdf, Light* light) {
    Color ambient, diffuse, specular;

    ambient = brdf->ka * light->color;
    
    Vector N = local->normal;
    Vector L = (light->position - local->position).normalize();
    diffuse = brdf->kd * N.dot(L) * light->color;
    
    Vector R = N*2*(L.dot(N)) - L;
    specular = brdf->ks * pow(R.dot(lookAt - local->position), brdf->kr) * light->color;

    Color final = ambient + diffuse + specular;
    return final;
    
}
