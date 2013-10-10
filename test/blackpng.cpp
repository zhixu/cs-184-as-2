#include "film.h"
#include "sampler.h"

/*
 *
 * I tried to compile this with:
 *  g++ -g -Wall -I include/ src/film.cpp test/blackpng.cpp src/primitives.cpp -lpng
 */

int main (int argc, char* argv[]) {
    Film* f = new Film(640, 480);

    Sample* s;
    Color* c = new Color(255, 0, 0);
    for(int i=0; i<480; i++){
        s = new Sample(i, i);
        f->commit(s, c);
        delete s;
    }

    f->write((char*) "poop.png");
}
