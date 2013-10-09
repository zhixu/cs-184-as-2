#include "film.h"

/*
 *
 * I tried to compile this with:
 *  g++ -g -Wall -I include/ src/film.cpp test/blackpng.cpp src/primitives.cpp -lpng
 */
int main (int argc, char* argv[]) {
    Film* f = new Film(640, 480);

    f->write((char*) "poop.png");
}
