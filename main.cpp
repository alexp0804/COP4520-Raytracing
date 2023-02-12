#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>

using namespace std;

Color ray_color(Ray r) {
    Vec3 unit_dir = normalized(r.direction());
    double t = 0.5 * (unit_dir.y() + 1.0);

    Color a = Color(1, 1, 1);
    Color b = Color(0.5, 0.7, 1.0);

    return (1 - t) * a + t * b;
}

int main() {
    // Image constants
    float aspect_ratio = 16.0 / 9.0;
    int img_width = 400;
    int img_height = (int) img_width / aspect_ratio;

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
            double u = (double) i / (img_width - 1);
            double v = (double) j / (img_height - 1);

            Color pixel = ray_color(
                Ray(
                    origin,
                    lower_left_corner + u * horizontal + v * vertical - origin
                )
            );

            write_color(std::cout, pixel);
        }
    }
    cerr << endl << "Done!" << endl;

    return 0;
}

