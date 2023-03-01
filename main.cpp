#include "rt_utilities.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

using namespace std;

Color ray_color(Ray r, const Hittable& world, int depth) {
    HitRecord rec;

    // When exceeding Ray Bounce Limit, stop gathering light
    if (depth <= 0) {
        return Color(0, 0, 0);
    }

    if (world.hit(r, 0.001, infinity, rec)) {
          Ray scattered;
          Color attenuation;
          if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, world, depth-1);
          return Color(0,0,0);
    }

    Vec3 unit_direction = normalized(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);

    Color a = Color(1.0, 1.0, 1.0);
    Color b = Color(0.5, 0.7, 1.0);

    return (1.0 - t) * a + t * b;
}

int main() {
    // Image constants
    double aspect_ratio = 16.0 / 9.0;
    int img_width = 400;
    int img_height = (int) img_width / aspect_ratio;
    int samples_per_pixel = 100;
    int max_depth = 50;

    // World
    HittableList world;
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    Camera cam;

    // Viewport constants
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    Point3 origin = Point3();
    Vec3 horizontal = Vec3(viewport_width, 0, 0);
    Vec3 vertical = Vec3(0, viewport_height, 0);
    Vec3 lower_left_corner = origin - horizontal * 0.5 - vertical * 0.5 - Vec3(0, 0, focal_length);

    // Render image
    cout << "P3" << endl << img_width << ' ' << img_height << endl << "255" << endl;

    for (int j = img_height - 1; j >= 0; j--)
    {
        cerr << "\rLines remaining: " << j << ' ' << flush;

        for (int i = 0; i < img_width; i++)
        {
            Color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (img_width - 1);
                auto v = (j + random_double()) / (img_height - 1);

                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    cerr << endl << "Done!" << endl;

    return 0;
}

