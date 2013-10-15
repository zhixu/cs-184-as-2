#include <math.h>
#include "raytracer.h"

#include <stdio.h>
#include <stdlib.h>

#define DEPTH_THRESHOLD 1

RayTracer::RayTracer (std::vector< Shape* > ss, std::vector< Light* > ls) {
    shapes = ss;
    lights = ls;
}

void RayTracer::trace(Ray ray, int depth, Color& color) {

    //printf("entering ray color r: %f  g: %f  b: %f\n", color.r, color.g, color.b);
    
    if(depth > DEPTH_THRESHOLD){
        // Color's default constructor makes it black,
        // no need to do anything here
        return;
    }

    float t_of_hit;
    LocalGeo intersection;
    Shape* shape;
    bool has_intersection = false;

    for(std::vector<int>::size_type i=0; has_intersection == false && i != shapes.size(); i++){
        shape = shapes[i];
 
        float t_of_hit;
        if(shape->intersect(ray, t_of_hit, intersection)){
            has_intersection = true;
            Brdf brdf; //added for testing
            brdf = shape->brdf; //added for testing
            Light* light;
            
            for(std::vector<int>::size_type i=0; i != lights.size(); i++){
                light = lights[i];
                illuminate(color, ray.position, intersection, brdf, light); //added
            }
            return; // break;
        }
    }

    if(!has_intersection){
        return;
    }
    /*
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
    }*/
}

void RayTracer::illuminate(Color& color, Point lookAt, LocalGeo local, Brdf brdf, Light* light) {
    Color ambient, diffuse, specular;
    
    //printf("pre illum colors r: %f  g: %f  b: %f\n", color.r, color.g, color.b);
    /* ambient */
    ambient = brdf.ka * light->color;
    //printf("brdf colors r: %f  g: %f  b: %f\n", brdf.ka.r, brdf.ka.g, brdf.ka.b);
    //printf("ambient colors r: %f  g: %f  b: %f\n", ambient.r, ambient.g, ambient.b);
    //printf("light colors: r: %f  g: %f  b: %f\n", light->color.r, light->color.g, light->color.b);
    
    /* diffuse */
    Vector N = local.normal.normalize();
    Vector L = (light->position - local.position).normalize();
    float diff = L.dot(N);
    diffuse = brdf.kd * light->color * diff;

    //printf("diffuse colors r: %f  g: %f  b: %f\n", diffuse.r, diffuse.g, diffuse.b);
    
    /* specular */
    Vector R = N*2*diff - L;
    Vector V = (lookAt - local.position).normalize();
    specular = brdf.ks * light->color * pow(R.dot(V), brdf.kr);
    specular *= diff;
    //printf("specular colors r: %f  g: %f  b: %f\n", specular.r, specular.g, specular.b);

    color += ambient;
    color += diffuse;
    color += specular;
    
    //color = Color(255, 255, 255);
    
    //printf("illum colors r: %f  g: %f  b: %f\n", color.r, color.g, color.b);
}
