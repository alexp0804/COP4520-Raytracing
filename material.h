#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt_utilities.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class Material {
public:
    virtual bool scatter(
        Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered
    ) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(Color a) : albedo(a) {}

        virtual bool scatter(
            Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Color albedo;
};

class Metal : public Material {
public:
    Metal(Color a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered
    ) const override {
          Vec3 reflected = reflect(normalized(r_in.direction()), rec.normal);
          Vec3 scatterDirection = random_in_unit_sphere();
          scattered = Ray(rec.p, reflected + fuzz * scatterDirection);
          attenuation = albedo;
          return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    Color albedo;
    double fuzz;
};

#endif