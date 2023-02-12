#ifndef COLOR
#define COLOR

#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, Color pixel_color) {
    out << (int) (255.999 * pixel_color.x()) << ' '
        << (int) (255.999 * pixel_color.y()) << ' '
        << (int) (255.999 * pixel_color.z()) << std::endl;
}

#endif