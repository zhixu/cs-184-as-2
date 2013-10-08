#include "primitives.h"

class Film {
    private:
        Color** colors;
    public:
        int width, height;
        Film(int, int);
        void commit (Sample&, Color&);
        void write (char*);
};
