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

#include <algorithm>
#include <iostream>
#include <future>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

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
    int img_height = img_width / aspect_ratio;
    int samples_per_pixel = 100;
    int max_depth = 50;


    // World

    HittableList world;

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera cam(Point3(-2, 2, 1), Point3(0, 0, -1), Vec3(0, 1, 0), 20, aspect_ratio);

    auto start = high_resolution_clock::now();
    // Render image
    cerr << "Rendering..." << endl;
    cout << "P3" << endl << img_width << ' ' << img_height << endl << "255" << endl;

    const int chunk_size = 16; // number of rows in each chunk
    const int num_chunks = (img_height + chunk_size - 1) / chunk_size; // number of chunks

    vector<future<vector<pair<int, Color>>>> futures(num_chunks);

    mutex mtx; // declare a mutex for proper storing.

    for (int chunk = 0; chunk < num_chunks; chunk++) {
        int start_row = chunk * chunk_size;
        int end_row = min(start_row + chunk_size, img_height);

        futures[chunk] = async(launch::async, [=, &cam, &world, &mtx]() {
            vector<pair<int, Color>> chunk_colors;

            for (int j = end_row - 1; j >= start_row; j--) {
                for (int i = 0; i < img_width; i++) {
                    Color pixel_color(0, 0, 0);

                    for (int s = 0; s < samples_per_pixel; ++s) {
                        auto u = (i + random_double()) / (img_width - 1);
                        auto v = (j + random_double()) / (img_height - 1);

                        Ray r = cam.get_ray(u, v);
                        pixel_color += ray_color(r, world, max_depth);
                    }

                    // acquire the lock before accessing shared data
                    lock_guard lock(mtx);
                    chunk_colors.emplace_back(j, pixel_color);
                }
            }

            return chunk_colors;
        });
    }

    vector<pair<int, Color>> all_colors;
    for (int chunk = num_chunks - 1; chunk >= 0; chunk--) {
        // Since the final result of the data is wrapped around by an std::future,
        // we have to retrieve it.
        auto chunk_colors = futures[chunk].get();

        // Append to the end of the vector.
        all_colors.insert(all_colors.end(), chunk_colors.begin(), chunk_colors.end());
    }

    for (auto& [row, color] : all_colors) {
        write_color(std::cout, color, samples_per_pixel);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cerr << endl << "Done!" << endl;
    cerr << "Took: " << duration.count() << " milliseconds" << endl;

    return 0;
}

