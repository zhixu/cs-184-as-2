#ifndef SCENE_H
#define SCENE_H

#include <string>

class Scene {
    private:
        int width, height;
        int maxDepth;
        std::string outputFilename;

        // camera stuff
        Point *lookFrom, *lookAt, *up;
        float fov;

        // shapes
        std::vector< Shape* > shapes;
        

        // lights
        std::vector< Light* > lights;

    public:
        Scene(std::string);

};

#endif
