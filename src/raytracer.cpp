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

    // there are 2 intersections we care about:
    // camera ray intersecting object
    // shadow ray intersecting blocker
    // Lets make variable names that differentiate between the two

    float objectHitT, blockerHitT;
    LocalGeo objectIntersection, blockerIntersection;
    Shape *object, *blocker;
    bool hitObject = false; // did the camera ray hit an object?

    bool hitBlocker; // did the shadow ray for an object hit a blocker?
                     // to be determined in an inner loop

    // variables for use while looking over objects
    Shape *tempObject;
    float tempObjectHitT;
    LocalGeo tempObjectIntersection;
    bool hitTemp;

    // get the closest object that the ray hits
    for(std::vector<int>::size_type i=0; i != shapes.size(); i++){
        tempObject = shapes[i];
        hitTemp = tempObject->intersect(ray, tempObjectHitT, tempObjectIntersection);
        if(hitTemp){
            if(!hitObject){
                //this is the first object to be hit
                objectIntersection = tempObjectIntersection;
                object = tempObject;
                objectHitT = tempObjectHitT;
                hitObject = true;
            } else if(tempObjectHitT < objectHitT){
                // we found a closer object hit!
                objectIntersection = tempObjectIntersection;
                object = tempObject;
                objectHitT = tempObjectHitT;
            }
        }
    }

    if(!hitObject){
        // we didn't hit anything, so nothing to do
        return;
    }

    // at this point, we know the camera ray hit an object
    // now we need to see which lights manage to hit it

    Brdf brdf = object->brdf;

    Light* light;
    Ray shadowRay;

    for(std::vector<int>::size_type i=0; i != lights.size(); i++){
        light = lights[i];
        light->generateShadowRay(objectIntersection, shadowRay, light->color); // sets shadowRay

        // to check if the light ray is blocked, loop over each of the other objects and check 
        // if the shadow ray hits a blocker
        hitBlocker = false;

        for(std::vector<int>::size_type j=0; hitBlocker == false && j != shapes.size(); j++){
            tempObject = shapes[j];
            if(tempObject == object){
                continue;
            }

            hitTemp = tempObject->intersect(shadowRay, tempObjectHitT, tempObjectIntersection);
            if(hitTemp){
                hitBlocker = true;
                blocker = tempObject;
                blockerHitT = tempObjectHitT;

                break;
            }
        }

        if(hitBlocker){
            // we still need to account for ambient light
            color += brdf.ka * light->color;
        } else {
            // do the full phong illumination!
            illuminate(color, ray.position, objectIntersection, brdf, light);
        }
    }

    // TODO: handle reflection
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
