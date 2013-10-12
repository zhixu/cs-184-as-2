#include <vector>
#include <math.h>
#include "primitives.h"

#define DEPTH_THRESHOLD 1

RayTracer::RayTracer (std::vector< Primitive* > ps, std::vector< Light* > ls) {
    primitives = ps;
    lights = ls;
}

void RayTracer::trace(Ray& ray, int depth, Color* color){
    if(depth > DEPTH_THRESHOLD){
        // Color's default constructor makes it black,
        // no need to do anything here
        return;
    }

    float t_of_hit;
    LocalGeo* intersection;
    Primitive* primitive;
    bool has_intersection = false;
    for(std::vector<int>::size_type i=0; has_intersection == false && i != primitives.size(); i++){
        primitive = primitives[i];

        if(primitive.intersect(ray, &t_of_hit, &intersection)){
            has_intersection = true;
            break;
        }
    }

    if(!has_intersection){
        return;
    }

    Brdf* brdf;
    intersection->primitive->getBrdf(intersection, &brdf);

    Light* light;
    Ray* lightRay;
    Color* lightColor;
    for(std::vector<int>::size_type i=0; i != lights.size(); i++){
        light = lights[i];
        light->generateLightRay(intersection, &lightRay, &lightColor);

        // check if the light ray is blocked
        if(!primitive->intersectP(lightRay)){
            *color += shading(intersection, brdf, lightRay, lightColor);
        }

        // TODO: handle reflection
    }
}

Color illuminate(Point lookAt, LocalGeo local, Brdf brdf, Light light, Color* ambient, Color* diffuse, Color* specular) {
    
    Color k = Color(brdf.ka, brdf.ka, brdf.ka);
    *ambient = k*light.c;
    
    Vector N = brdf.normal;
    Vector L = (light.pos - brdf.position).normalize();
    *diffuse = brdf.kd * N.dot(L) * light.c;
    
    Vector R = N*2*(L.dot(N)) - L;
    *specular = brdf.ks * pow(R.dot(lookAt), kr) * light.c;
    
}