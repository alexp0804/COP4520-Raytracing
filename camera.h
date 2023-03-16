#ifndef CAMERA_H
#define CAMERA_H

#include "rt_utilities.h"
#include "vec3.h"
#include "ray.h"

class Camera {
public:
    Camera(const Point3 look_from, const Point3 look_at, const Vec3 up, const double vert_fov, const double aspect_ratio) {
        auto theta = degrees_to_radians(vert_fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = normalized(look_from - look_at);
        auto u = normalized(cross(up, w));
        auto v = cross(w, u);

        origin = look_from;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    [[nodiscard]] Ray get_ray(const double s, const double t) const {
        return { origin, lower_left_corner + s * horizontal + t * vertical - origin };
    }

private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif