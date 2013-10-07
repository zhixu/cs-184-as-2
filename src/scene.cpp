
void render() {
        while (!sampler.generateSample(&sample) {
            camera.generateRay(sample, &ray);
            raytracer.trace(ray, &color);
            film.commit(sample, color);
            }
            film.writeImage();
    }
