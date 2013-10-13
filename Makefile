all:
	g++ -g -Wall -I include/ src/film.cpp  src/primitives.cpp src/sampler.cpp src/scene.cpp src/shapes.cpp src/raytracer.cpp -lpng
