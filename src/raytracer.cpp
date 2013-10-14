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
    brdf = shape->brdf;

    Light* light;
    Ray* shadowRay;
    for(std::vector<int>::size_type i=0; i != lights.size(); i++){
        light = lights[i];
        light->generateShadowRay(intersection, shadowRay, &light->color);

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
            illuminate(color, &ray->position, intersection, brdf, light);
        }

        // TODO: handle reflection
    }
}

void RayTracer::illuminate(Color* color, Point* lookAt, LocalGeo* local, Brdf* brdf, Light* light) {
    Color *ambient, *diffuse, *specular;

    ambient = new Color (brdf->ka.r * light->color.r,
                         brdf->ka.g * light->color.g,
                         brdf->ka.b * light->color.b);
    
    Vector N = local->normal;
    Vector L = (light->position - local->position).normalize();
    float diff = N.dot(L);
    diffuse = new Color(brdf->kd.r * light->color.r,
                        brdf->kd.g * light->color.g,
                        brdf->kd.b * light->color.b);
    *diffuse *= diff;
    
    Vector R = N*2*(L.dot(N)) - L;
    diff = pow(R.dot(*lookAt - local->position), brdf->kr);
    specular = new Color(brdf->ks.r * light->color.r,
                        brdf->ks.g * light->color.g,
                        brdf->ks.b * light->color.b);
    *specular *= diff;

    *color += *ambient;
    *color += *diffuse;
    *color += *specular;
}
