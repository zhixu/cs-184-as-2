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

    float distanceToObject;
    LocalGeo objectIntersection, blockerIntersection;
    Shape *object;
    bool hitObject = false; // did the camera ray hit an object?

    bool hitBlocker; // did the shadow ray for an object hit a blocker?
                     // to be determined in an inner loop

    // variables for use while looking over objects
    Shape *tempObject;
    float tempObjectHitT, tempDistance;
    LocalGeo tempObjectIntersection;
    bool hitTemp;

    // get the closest object that the ray hits
    for(std::vector<int>::size_type i=0; i != shapes.size(); i++){
        tempObject = shapes[i];
        hitTemp = tempObject->intersect(ray, tempObjectHitT, tempObjectIntersection);
        if(hitTemp){
            tempDistance = (tempObjectIntersection.position - ray.position).magnitude();
            if(!hitObject){
                //this is the first object to be hit
                objectIntersection = tempObjectIntersection;
                object = tempObject;
                hitObject = true;
                distanceToObject = tempDistance;
            } else if (tempDistance < distanceToObject){
                // we found a closer object hit!
                objectIntersection = tempObjectIntersection;
                object = tempObject;
                distanceToObject = tempDistance;
            }
        }
    }

    if(!hitObject){
        // we didn't hit anything, so nothing to do
        return;
    }

    // at this point, we know the camera ray hit an object
    // now we need to see which lights manage to hit it

    Light* light;
    Ray shadowRay;
    Brdf brdf = object->brdf;
    color += brdf.emission;
        
    for(std::vector<int>::size_type i=0; i != lights.size(); i++){
        light = lights[i];
        light->generateShadowRay(objectIntersection, shadowRay, light->color); // sets shadowRay

        // to check if the light ray is blocked, loop over each of the other objects and check 
        // if the shadow ray hits a blocker
        hitBlocker = false;

        for(std::vector<int>::size_type j=0; hitBlocker == false && j != shapes.size(); j++){
            tempObject = shapes[j];
            if(tempObject == object){ continue; }

            hitTemp = tempObject->intersect(shadowRay, tempObjectHitT, tempObjectIntersection);
            if(hitTemp){
                if (shadowRay.t_max < 1) {
                    hitBlocker = true;
                    break;
                }
            }
        }
        if(hitBlocker){
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
    
        
    /* --------------------------- ambient --------------------------- */
    ambient = brdf.ka * light->color;
    
    /* --------------------------- diffuse --------------------------- */

    diffuse = brdf.kd * light->color * LN;
    
    /* --------------------------- specular --------------------------- */

    specular = brdf.ks * light->color * pow(RV, brdf.sp);

    color += ambient;
    color += diffuse;
    if(LN > 0){ // prevents adding a specular component to the wrong side of the object
        color += specular;
        //printf("specular\t");
        //specular.print();
    }
    /*
    printf("normal in illuminate\t");
    N.print();
    printf("light position\t");
    L.print();
    printf("LN \t %f\n", LN);
    printf("R \t");
    R.print();
    printf("V \t");
    V.print();
    printf("RV \t %f\n", RV);
    printf("ambient\t");
    ambient.print();
    printf("diffuse\t");
    diffuse.print();
    */
}

Ray RayTracer::createReflectRay(LocalGeo intersection, Ray ray){
    Vector r, d, n;
    d = ray.direction.normalize();
    n = intersection.normal.normalize();
    
    /*printf("EYE RAY DIRECTION \t");
    d.print();
    printf("NORMAL \t");
    n.print();*/

    r = (d - (n * 2 * d.dot(n))).normalize();
    
    //printf("REFLECTION DIRECTION \t");
    //r.print();

    //r.print();
    //d.print();
    //printf("\n\n");
    return Ray(intersection.position, r, 0.0001, 900);
}
