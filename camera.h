#ifndef CAMERA_H
#define CAMERA_H

#include "rt_utilities.h"
#include "vec3.h"

class Camera {
public:
    Camera() {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = Point3(0, 0, 0);
        horizontal = Vec3(viewport_width, 0.0, 0.0);
        vertical = Vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);
    }

    Ray get_ray(double u, double v) const {
        auto hor = horizontal;
        auto vert = vertical;
        auto llc = lower_left_corner;
        auto o = origin;

        return Ray(
            origin,
            llc + u * hor + v * vert - o
        );    
    }

private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif