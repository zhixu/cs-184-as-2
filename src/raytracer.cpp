#include <math.h>
#include "raytracer.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

RayTracer::RayTracer (std::vector< Shape* > ss, std::vector< Light* > ls) {
    shapes = ss;
    lights = ls;
}

void RayTracer::trace(Ray ray, int depth, Color& color) {
    // to make things easier, the depth is initially maxDepth, and counts down to 0

    if(depth < 0){
        // Color's default constructor makes it black,
        // no need to do anything here
        return;
    }

    // there are 2 intersections we care about:
    // camera ray intersecting object
    // shadow ray intersecting blocker
    // Lets make variable names that differentiate between the two

    float objectHitT;//, blockerHitT;
    LocalGeo objectIntersection, blockerIntersection;
    Shape *object;//, *blocker;
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
    //printf("OUTSIDE brdf ka r: %f  g: %f  b: %f\n", brdf.ka.r, brdf.ka.g, brdf.ka.b);

    color += brdf.emission;

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
                //blocker = tempObject;
                //blockerHitT = tempObjectHitT;

                break;
            }
        }

        if(hitBlocker){
            // we still need to account for ambient light
            //color += brdf.ka * light->color;
            break;
        } else {
            // do the full phong illumination!
            illuminate(color, ray.position, objectIntersection, brdf, light);
        }
    }

    if(brdf.kr.isNotZero()){
        Ray reflectRay = createReflectRay(objectIntersection, ray);
        Color tempColor = Color(0, 0, 0);
        trace(reflectRay, depth - 1, tempColor);
        Color reflection = tempColor * brdf.kr;
   //     printf("Reflection\t"); reflection.print();
        color += reflection;
    }
 //   printf("Final\t\t");
   // color.print();
  //  printf("\n");

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
    //printf("normal in illuminate\t");
    //N.print();
        
    /* --------------------------- ambient --------------------------- */
    ambient = brdf.ka * light->color;
    //printf("ambient\t\t"); ambient.print();
    //printf("brdf ka r: %f  g: %f  b: %f\n", brdf.ka.r, brdf.ka.g, brdf.ka.b);
    
    /* --------------------------- diffuse --------------------------- */

    diffuse = brdf.kd * light->color * LN;
    //printf("diffuse\t\t"); diffuse.print();
    //printf("LN %f\n", LN);
    
    /* --------------------------- specular --------------------------- */

    specular = brdf.ks * light->color * pow(RV, brdf.sp);
    //printf("specular\t"); specular.print();
    //printf("RV %f\n", RV);

    color += ambient;
    color += diffuse;
    if(LN > 0){ // prevents adding a specular component to the wrong side of the object
        color += specular;
        /*printf("specular\t");
        specular.print();*/
    }
    
    /*printf("triangle colors\t");
    color.print();
    printf("ambient\t");
    ambient.print();
    printf("diffuse\t");
    diffuse.print();*/
    //printf("illum colors r: %f  g: %f  b: %f\n", color.r, color.g, color.b);
    //printf("phong\t\t"); color.print();
}

Ray RayTracer::createReflectRay(LocalGeo intersection, Ray ray){
    Vector r, d, n;
    d = ray.direction.normalize();
    n = intersection.normal.normalize();

    r = d - ((n * 2) * d.dot(n));

    //r.print();
    //d.print();
    //printf("\n\n");
    return Ray(intersection.position, r, 0.05, 0);
}
