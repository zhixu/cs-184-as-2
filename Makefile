all:
	g++ -g -Wall -O2 -I include/ src/film.cpp  src/primitives.cpp src/sampler.cpp src/scene.cpp src/shapes.cpp src/raytracer.cpp -lpng
