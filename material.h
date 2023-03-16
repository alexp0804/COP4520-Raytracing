#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "rt_utilities.h"
#include "vec3.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(
        Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered
    ) const = 0;
};

class Lambertian final : public Material {
public:
	explicit Lambertian(Color a) : albedo(a) {}

	bool scatter(Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered) const override {
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

class Metal final : public Material {
public:
    Metal(Color a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered) const override {
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

class Dielectric final : public Material
{
public:
	explicit Dielectric(const double index_of_refraction): ir(index_of_refraction){}

    bool scatter(Ray r_in, HitRecord& rec, Color& attenuation, Ray& scattered) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? 1.0 / ir : ir;

        Vec3 unit_direction = normalized(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool unrefractable = refraction_ratio * sin_theta > 1.0;

		Vec3 direction = unrefractable || reflectance(cos_theta, refraction_ratio) > random_double() ?
            reflect(unit_direction, rec.normal) :
			refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray(rec.p, direction);

        return true;
    }

    double ir; // Index of Refraction

private:
    static double reflectance(const double cos, const double idx_ref)
    {
	    // Schlick approximation
        auto r0 = (1 - idx_ref) / (1 + idx_ref);
        r0 *= r0;
        return r0 + (1 - r0) * pow((1 - cos), 5);
    }
};

#endif