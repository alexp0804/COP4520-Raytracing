#ifndef CAMERA_H
#define CAMERA_H

#include "rt_utilities.h"
#include "vec3.h"
#include "ray.h"

class Camera {
public:
    Camera( const Point3 look_from,
			const Point3 look_at,
			const Vec3 up,
			const double vert_fov,
			const double aspect_ratio,
			const double aperture,
			const double focus_dist) {

        auto theta = degrees_to_radians(vert_fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = normalized(look_from - look_at);
        u = normalized(cross(up, w));
        v = cross(w, u);

        origin = look_from;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2;
    }

    [[nodiscard]] Ray get_ray(const double s, const double t) const {

        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = u * rd.x() + v * rd.y();

        return { origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset };
    }

private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lens_radius;
};

#endif