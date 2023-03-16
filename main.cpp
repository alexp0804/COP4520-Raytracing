#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <vector>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "rt_utilities.h"
#include "Sphere.h"
#include "vec3.h"

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

HittableList random_scene() {
    HittableList world;

    auto ground_Material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_Material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_Material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_Material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_Material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_Material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_Material));
                }
                else {
                    // glass
                    sphere_Material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_Material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int img_width = 1200;
    const int img_height = static_cast<int>(img_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World

    auto world = random_scene();

    // Camera

    Point3 look_from(13, 2, 3);
    Point3 look_at(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(look_from, look_at, vup, 20, aspect_ratio, aperture, dist_to_focus);

    auto start = high_resolution_clock::now();
    // Render image
    cerr << "Rendering..." << endl;
    cout << "P3" << endl << img_width << ' ' << img_height << endl << "255" << endl;

    auto num_cores = thread::hardware_concurrency();
    double ideal_chunk_size = sqrt(img_height * num_cores);
    int chunk_size = static_cast<int>(round(ideal_chunk_size)); // number of rows in each chunk
    int num_chunks = (img_height + chunk_size - 1) / chunk_size; // number of chunks

    vector<future<vector<pair<int, Color>>>> futures(num_chunks);

    mutex mtx; // declare a mutex for proper storing.

    for (int chunk = 0; chunk < num_chunks; chunk++) {
        int start_row = chunk * chunk_size;
        int end_row = min(start_row + chunk_size, img_height);

        futures[chunk] = async(launch::async, [=, &cam, &world, &mtx]() {
            vector<pair<int, Color>> chunk_Colors;

            for (int j = end_row - 1; j >= start_row; j--) {
                for (int i = 0; i < img_width; i++) {
                    Color pixel_Color(0, 0, 0);

                    for (int s = 0; s < samples_per_pixel; ++s) {
                        auto u = (i + random_double()) / (img_width - 1);
                        auto v = (j + random_double()) / (img_height - 1);

                        Ray r = cam.get_ray(u, v);
                        pixel_Color += ray_color(r, world, max_depth);
                    }

                    // acquire the lock before accessing shared data
                    lock_guard lock(mtx);
                    chunk_Colors.emplace_back(j, pixel_Color);
                }
            }

            return chunk_Colors;
        });
    }

    vector<pair<int, Color>> all_Colors;
    for (int chunk = num_chunks - 1; chunk >= 0; chunk--) {
        // Since the final result of the data is wrapped around by an std::future,
        // we have to retrieve it.
        auto chunk_Colors = futures[chunk].get();

        // Append to the end of the vector.
        all_Colors.insert(all_Colors.end(), chunk_Colors.begin(), chunk_Colors.end());
    }

    for (auto& [row, Color] : all_Colors) {
        write_color(std::cout, Color, samples_per_pixel);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cerr << endl << "Done!" << endl;
    cerr << "Took: " << duration.count() << " milliseconds" << endl;

    return 0;
}

