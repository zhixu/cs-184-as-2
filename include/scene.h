#ifndef SCENE_H
#define SCENE_H

#include <string>

class Scene {
    public:
        int width, height;
        int maxDepth;
        std::string outputFilename;

        // camera stuff
        Point *lookFrom, *lookAt;
        Vector *up;
        float fov;

        // shapes
        std::vector< Shape* > shapes;

        // lights
        std::vector< Light* > lights;
        
        Brdf brdf;

        Scene(std::string);

};

#endif
