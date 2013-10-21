#ifndef SCENE_H
#define SCENE_H

#include <string>

class Scene {
    public:
        int width, height;
        int maxDepth;
        int raysPerPixel;
        std::string outputFilename;

        // camera stuff
        Point lookFrom, lookAt;
        Vector up;
        float fov;

        // shapes
        std::vector< Shape* > shapes;
        std::vector< Point* > points; // for triangles

        // lights
        std::vector< Light* > lights;
        
        //transformation
        //std::stack<Matrix> mst;

        Scene(std::string);

};

#endif
