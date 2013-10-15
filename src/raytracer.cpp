#include <math.h>
#include "raytracer.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    float t_of_shadow_hit;
    LocalGeo intersection;
    Shape *shape, *subshape;
    bool has_intersection = false;

    for(std::vector<int>::size_type i=0; has_intersection == false && i != shapes.size(); i++){
        shape = shapes[i];
        float t_of_hit;
        
        if(shape->intersect(ray, t_of_hit, intersection)){
            has_intersection = true;
            Brdf brdf = shape->brdf;
            bool intersectsOtherObject;
            
            /* --------------------------- shadow rays --------------------------- */
            Light* light;
            Ray shadowRay;
            
            for(std::vector<int>::size_type i=0; i != lights.size(); i++){
                intersectsOtherObject = false;
                light = lights[i];
                light->generateShadowRay(intersection, shadowRay, light->color);

                // check if the light ray is blocked
                
                // loop over each of the other objects and check if this shadow ray hits
                for(std::vector<int>::size_type j=0; intersectsOtherObject == false && j != shapes.size(); j++){
                    subshape = shapes[j];
                    
                    if(subshape->intersect(shadowRay, t_of_shadow_hit, intersection)) {
                        intersectsOtherObject = true;
 //                       color += brdf.ka * light->color; //don't skip ambient
                        break;
                    }

                    //phong phong
                    if(!intersectsOtherObject) {
                        illuminate(color, ray.position, intersection, brdf, light);
                        break;
                    }
                }
            }
        }
        if (!has_intersection) {
            break;
        }

        // TODO: handle reflection
    }
    return;
}

void RayTracer::illuminate(Color& color, Point lookAt, LocalGeo local, Brdf brdf, Light* light) {
    
    Color ambient, diffuse, specular;
    
    Vector L = light->getLm(local.position).normalize();
    Vector N = local.normal.normalize();
    float LN = L.dot(N);
    Vector R = (N*2*LN - L).normalize();
    Vector V = (lookAt - local.position).normalize();
    float RV = R.dot(V);
    //printf("LN %f\n", LN);
        
    /* --------------------------- ambient --------------------------- */
    ambient = brdf.ka * light->color;
    //printf("brdf ka r: %f  g: %f  b: %f\n", brdf.ka.r, brdf.ka.g, brdf.ka.b);
    //printf("lightcolor r: %f  g: %f  b: %f\n", light->color.r, light->color.g, light->color.b);
    //printf("ambient colors r: %f  g: %f  b: %f\n", ambient.r, ambient.g, ambient.b);
    
    /* --------------------------- diffuse --------------------------- */

    diffuse = brdf.kd * light->color * LN;
    /*printf("LN2 %f\n", LN);
    printf("L x: %f y: %f z: %f \n", L.x, L.y, L.z);
    printf("N x: %f y: %f z: %f \n", N.x, N.y, N.z);
    printf("brdf kd r: %f  g: %f  b: %f\n", brdf.kd.r, brdf.kd.g, brdf.kd.b);*/
    //printf("diffuse colors r: %f  g: %f  b: %f\n", diffuse.r, diffuse.g, diffuse.b);
    
    /* --------------------------- specular --------------------------- */

    specular = brdf.ks * light->color * pow(RV, brdf.kr);
    //printf("RV %f\n", RV);
    //printf("specular colors r: %f  g: %f  b: %f\n", specular.r, specular.g, specular.b);

    color += ambient;
    color += diffuse;
    color += specular;
    

    
    
    
    //color = Color(255, 255, 255);
    
    //printf("illum colors r: %f  g: %f  b: %f\n", color.r, color.g, color.b);
}
