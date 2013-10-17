#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <queue>

class Scene {
    public:
        int width, height;
        int maxDepth;
        std::string outputFilename;

        // camera stuff
        Point lookFrom, lookAt;
        Vector up;
        float fov;

        // shapes
        std::vector< Shape* > shapes;
        Point *points; //for triangles
        int p_counter;

        // lights
        std::vector< Light* > lights;
        
        //transformation
        std::queue<Matrix> mst;

        Scene(std::string);

};

#endif
