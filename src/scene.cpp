#include <stdio.h>
#include <stdlib.h>

//this set of includes are from loadscene.cpp
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <stack>

#include "sampler.h"
#include "film.h"
#include "shapes.h"
#include "scene.h"
#include "raytracer.h"




int main (int argc, char* argv[]) {
    if(argc < 2){
        // we need an input filename, so print error and exit program
        printf("Need input filename as first argument. Exiting.");
        return 1;
    }
    std::string inputFilename = argv[1];
    Scene scene = Scene(inputFilename);
    Film film = Film(scene.width, scene.height);

    RayTracer rayTracer = RayTracer(scene.shapes, scene.lights);
    Sample sampleGenerator = Sample(scene.lookFrom,
                                    scene.lookAt,
                                    scene.up,
                                    scene.fov,
                                    film);

    int x, y;
    Point p;
    
    Ray r = Ray();

    r.t_min = 0;
    r.t_max = 900;

    r.position = scene.lookFrom;

    for(x=0; x<scene.width; x++){
        for(y=0; y<scene.height; y++){
            Color c = Color(0, 0, 0);
            p = sampleGenerator.getSample(x, y);
            r.direction = p - scene.lookFrom;
            //printf("Scene color: r %f  g %f  b %f \n", c.r, c.g, c.b);
            //printf("coords width %d  y %d   ", y, x);
            //printf("direction x: %f  y: %f z: %f\n", r.direction.x, r.direction.y, r.direction.z);
            
            rayTracer.trace(r, scene.maxDepth, c);

           film.commit(x, y, c);
        }
    }

    printf("%d %d\n", scene.width, scene.height);

    film.write((char *)scene.outputFilename.c_str());
    
    return 0;
}


Scene::Scene(std::string file) {
    // handle defaults
    maxDepth = 5;
    outputFilename = "output.png";

    float sp = 0;
    Color diffuse = Color(0, 0, 0);
    Color specular = Color(0, 0, 0);
    Color ambient = Color(0.2, 0.2, 0.2);
    Color emission = Color(0, 0, 0);



  std::ifstream inpfile(file.c_str());
  if(!inpfile.is_open()) {
    std::cout << "Unable to open file" << std::endl;
  } else {
    std::string line;
    //MatrixStack mst;
    std::stack< Matrix* > transformsStack;
    Matrix currentTransform = Matrix();
    currentTransform.identity();

    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;

      std::getline(inpfile,line);
      std::stringstream ss(line);
      

    printf("Processing: %s\n", line.c_str());

      while (ss >> buf) {
        splitline.push_back(buf);
      }
      //Ignore blank lines
      if(splitline.size() == 0) {
        continue;
      }

      //Ignore comments
      if(splitline[0][0] == '#') {
        continue;
      }

      //Valid commands:
      //size width height
      //  must be first command of file, controls image size
      else if(!splitline[0].compare("size")) {
        width = atoi(splitline[1].c_str());
        height = atoi(splitline[2].c_str());
      }
      //maxdepth depth
      //  max # of bounces for ray (default 5)
      else if(!splitline[0].compare("maxdepth")) {
        maxDepth = atoi(splitline[1].c_str());
      }
      //output filename
      //  output file to write image to 
      else if(!splitline[0].compare("output")) {
        outputFilename = splitline[1];
      }

      //camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
      //  speciﬁes the camera in the standard way, as in homework 2.
      else if(!splitline[0].compare("camera")) {
          int x, y, z;
        // lookfrom:
            x = atof(splitline[1].c_str());
            y = atof(splitline[2].c_str());
            z = atof(splitline[3].c_str());
            lookFrom = Point(x, y, z);
        // lookat:
            x = atof(splitline[4].c_str());
            y = atof(splitline[5].c_str());
            z = atof(splitline[6].c_str());
            lookAt = Point(x, y, z);
        // up:
            x = atof(splitline[7].c_str());
            y = atof(splitline[8].c_str());
            z = atof(splitline[9].c_str());
            up = Vector(x, y, z);
        // fov:
            fov = atof(splitline[10].c_str());
      }

      //sphere x y z radius
      //  Deﬁnes a sphere with a given position and radius.
      else if(!splitline[0].compare("sphere")) {
          float x, y, z, r;

        x = atof(splitline[1].c_str());
        y = atof(splitline[2].c_str());
        z = atof(splitline[3].c_str());
        Point center = Point(x, y, z);
        printf("circle parse middle: x %f  y %f  z %f\n", x, y, z);
        r = atof(splitline[4].c_str());

        // TODO: all of these fancy things. just storing shape dimensions for now
        // Create new sphere:
        //   Store 4 numbers
        //   Store current property values
        //   Store current top of matrix stack
        Shape* s = new Sphere(center, r);
        s->brdf = Brdf(diffuse, specular, ambient, emission, sp);
        shapes.push_back(s);
      }
      //maxverts number
      //  Deﬁnes a maximum number of vertices for later triangle speciﬁcations. 
      //  It must be set before vertices are deﬁned.
      else if(!splitline[0].compare("maxverts")) {
        // Care if you want
        // Here, either declare array size
        // Or you can just use a STL vector, in which case you can ignore this
        int x = atoi(splitline[1].c_str());
        points.reserve(x);
      }
      //maxvertnorms number
      //  Deﬁnes a maximum number of vertices with normals for later speciﬁcations.
      //  It must be set before vertices with normals are deﬁned.
      else if(!splitline[0].compare("maxvertnorms")) {
        // Care if you want
      }
      //vertex x y z
      //  Deﬁnes a vertex at the given location.
      //  The vertex is put into a pile, starting to be numbered at 0.
      else if(!splitline[0].compare("vertex")) {
        float x, y, z;
        
        x = atof(splitline[1].c_str()),
        y = atof(splitline[2].c_str()),
        z = atof(splitline[3].c_str());
        // Create a new vertex with these 3 values, store in some array
        Point* p = new Point(x, y, z);
        points.push_back(p);
      }
      //vertexnormal x y z nx ny nz
      //  Similar to the above, but deﬁne a surface normal with each vertex.
      //  The vertex and vertexnormal set of vertices are completely independent
      //  (as are maxverts and maxvertnorms).
      else if(!splitline[0].compare("vertexnormal")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // nx: atof(splitline[4].c_str()),
        // ny: atof(splitline[5].c_str()),
        // nz: atof(splitline[6].c_str()));
        // Create a new vertex+normal with these 6 values, store in some array
      }
      //tri v1 v2 v3
      //  Create a triangle out of the vertices involved (which have previously been speciﬁed with
      //  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
      //  should internally compute a face normal for this triangle.
      else if(!splitline[0].compare("tri")) {
          int v1, v2, v3;
          
        v1 = atoi(splitline[1].c_str());
        v2 = atoi(splitline[2].c_str());
        v3 = atoi(splitline[3].c_str());
        // Create new triangle:
        //   Store pointer to array of vertices
        
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
        Shape* s = new Triangle(*points[v1], *points[v2], *points[v3]);
        s->brdf = Brdf(diffuse, specular, ambient, emission, sp);
        shapes.push_back(s);
        

      }
      //trinormal v1 v2 v3
      //  Same as above but for vertices speciﬁed with normals.
      //  In this case, each vertex has an associated normal, 
      //  and when doing shading, you should interpolate the normals 
      //  for intermediate points on the triangle.
      else if(!splitline[0].compare("trinormal")) {
        // v1: atof(splitline[1].c_str())
        // v2: atof(splitline[2].c_str())
        // v3: atof(splitline[3].c_str())
        // Create new triangle:
        //   Store pointer to array of vertices (Different array than above)
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
      }

      //translate x y z
      //  A translation 3-vector
      else if(!splitline[0].compare("translate")) {
        float x, y, z;
        x = atof(splitline[1].c_str());
        y = atof(splitline[2].c_str());
        z = atof(splitline[3].c_str());
        // Update top of matrix stack
        Matrix m = Matrix();
        m.translate(x, y, z);
        currentTransform = currentTransform * m;
      }
      //rotate x y z angle
      //  Rotate by angle (in degrees) about the given axis as in OpenGL.
      else if(!splitline[0].compare("rotate")) {
        float x, y, z, angle;
        x = atof(splitline[1].c_str());
        y = atof(splitline[2].c_str());
        z = atof(splitline[3].c_str());
        angle = atof(splitline[4].c_str());
        // Update top of matrix stack
        Matrix m = Matrix();
        m.rotate(x, y, z, angle);
        currentTransform = currentTransform * m;
      }
      //scale x y z
      //  Scale by the corresponding amount in each axis (a non-uniform scaling).
      else if(!splitline[0].compare("scale")) {
        float x, y, z;
        x = atof(splitline[1].c_str());
        y = atof(splitline[2].c_str());
        z = atof(splitline[3].c_str());
        // Update top of matrix stack
        Matrix m = Matrix();
        m.scale(x, y, z);
        currentTransform = currentTransform * m;
      }
      //pushTransform
      //  Push the current modeling transform on the stack as in OpenGL. 
      //  You might want to do pushTransform immediately after setting 
      //   the camera to preserve the “identity” transformation.
      else if(!splitline[0].compare("pushTransform")) {
          Matrix* savedTransform = new Matrix(currentTransform);
            transformsStack.push(savedTransform);
      }
      //popTransform
      //  Pop the current transform from the stack as in OpenGL. 
      //  The sequence of popTransform and pushTransform can be used if 
      //  desired before every primitive to reset the transformation 
      //  (assuming the initial camera transformation is on the stack as 
      //  discussed above).
      else if(!splitline[0].compare("popTransform")) {
        if(!transformsStack.empty()){
            Matrix* topTransform = transformsStack.top();
            transformsStack.pop();
            currentTransform = *topTransform;
            delete topTransform;
        } else {
            std::cerr << "Tried to popTransform when there was nothing on the transform stack" << std::endl;
        }
      }

      //directional x y z r g b
      //  The direction to the light source, and the color, as in OpenGL.
      else if(!splitline[0].compare("directional")) {
        float x, y, z, r, g, b;
        x = atof(splitline[1].c_str());
        y = atof(splitline[2].c_str());
        z = atof(splitline[3].c_str());
        r = atof(splitline[4].c_str());
        g = atof(splitline[5].c_str());
        b = atof(splitline[6].c_str());
        Point p = Point(x, y, z);
        Color c = Color(r, g, b);
        lights.push_back(new DirectionalLight(p, c));
      }
      //point x y z r g b
      //  The location of a point source and the color, as in OpenGL.
      else if(!splitline[0].compare("point")) {
        float x, y, z, r, g, b;
        x = atof(splitline[1].c_str());
        y = atof(splitline[2].c_str());
        z = atof(splitline[3].c_str());
        r = atof(splitline[4].c_str());
        g = atof(splitline[5].c_str());
        b = atof(splitline[6].c_str());
        Point p = Point(x, y, z);
        Color c = Color(r, g, b);
        lights.push_back(new PointLight(p, c));
      }
      //attenuation const linear quadratic
      //  Sets the constant, linear and quadratic attenuations 
      //  (default 1,0,0) as in OpenGL.
      else if(!splitline[0].compare("attenuation")) {
        // const: atof(splitline[1].c_str())
        // linear: atof(splitline[2].c_str())
        // quadratic: atof(splitline[3].c_str())
      }
      //ambient r g b
      //  The global ambient color to be added for each object 
      //  (default is .2,.2,.2)
      else if(!splitline[0].compare("ambient")) {
          float r, g, b;
         r = atof(splitline[1].c_str());
         g = atof(splitline[2].c_str());
         b = atof(splitline[3].c_str());
         printf("brdf colors r: %f  g: %f  b: %f\n", r, g, b);
         ambient = Color(r, g, b);
      }

      //diffuse r g b
      //  speciﬁes the diﬀuse color of the surface.
      else if(!splitline[0].compare("diffuse")) {
          float r, g, b;
         r = atof(splitline[1].c_str());
         g = atof(splitline[2].c_str());
         b = atof(splitline[3].c_str());
         diffuse = Color(r, g, b);
      }
      //specular r g b 
      //  speciﬁes the specular color of the surface.
      else if(!splitline[0].compare("specular")) {
          float r, g, b;
         r = atof(splitline[1].c_str());
         g = atof(splitline[2].c_str());
         b = atof(splitline[3].c_str());
         specular = Color(r, g, b);
      }
      //shininess s (sp)
      //  speciﬁes the shininess of the surface.
      else if(!splitline[0].compare("shininess")) {
          float shininess;
         shininess = atof(splitline[1].c_str());
         sp = shininess;
      }
      //emission r g b
      //  gives the emissive color of the surface.
      else if(!splitline[0].compare("emission")) {
        int r, g, b;
        r = atof(splitline[1].c_str());
        g = atof(splitline[2].c_str());
        b = atof(splitline[3].c_str());
        // Update current properties
        emission = Color(r, g, b);
      } else {
        std::cerr << "Unknown command: " << splitline[0] << std::endl;
      }
    }

    inpfile.close();
  }

}

