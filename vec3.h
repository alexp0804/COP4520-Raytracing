#ifndef VEC3
#define VEC3

#include <cmath>
#include <iostream>
#include "rt_utilities.h"

constexpr auto ENTRIES = 3;

class Vec3 {
    // Vec3 fields
    public:
        double e[ENTRIES];

    // Vec3 methods
    Vec3() {
      for (double& i : e)
          i = static_cast<double>(0);
    }

	Vec3(const double x, const double y, const double z) {
      e[0] = x;
      e[1] = y;
      e[2] = z;
    }

    [[nodiscard]] double x() const { return e[0]; }

    [[nodiscard]] double y() const { return e[1]; }

    [[nodiscard]] double z() const { return e[2]; }

    [[nodiscard]] double length_squared() const
    {
      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    [[nodiscard]] double length() const
    {
      return std::sqrt(length_squared());
    }

    [[nodiscard]] bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    // Vec3 operators

    // Negate
    Vec3 operator- () const { return {-e[0], -e[1], -e[2]}; }

    // Subscript
    double operator[] (const int i) const { return e[i]; }
    double& operator[] (const int i) { return e[i]; }

    // Assign-add
    Vec3& operator+= (const Vec3 &v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];

      return *this;
    }
    // Assign-subtract
    Vec3 &operator-= (const Vec3 &v) {
      return *this += (-v);
    }
    // Assign-multiply (by scalar)
    Vec3& operator*= (const double s) {
      e[0] *= s;
      e[1] *= s;
      e[2] *= s;

      return *this;
    }
    // Assign-divide (by scalar)
    Vec3& operator/= (const double s) {
      e[0] /= s;
      e[1] /= s;
      e[2] /= s;

      return *this;
    }

    // Add
	Vec3 operator+ (const Vec3 &v) const
    {
      return {e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]};
    }
    // Subtract
	Vec3 operator- (const Vec3 &v) const
    {
      return {e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]};
    }

    // Multiply (by scalar)
	Vec3 operator* (const double s) const
    {
      return {e[0] * s, e[1] * s, e[2] * s};
    }
    // Divide (by scalar)
	Vec3 operator/ (const double s) const
    {
      return {e[0] / s, e[1] / s, e[2] / s};
    }

    // Multiply (by vector, element-wise)
	Vec3 operator* (const Vec3 &v) const
    {
      return {e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]};
    }

    // Print
	std::ostream& operator<< (std::ostream &out) const
    {
      return out << e[0] << ' ' << e[1] << ' ' << e[2] << std::endl;
    }

    // Randoms
	static Vec3 random() {
      return {random_double(), random_double(), random_double()};
    }

	static Vec3 random(const double min, const double max) {
      return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }
};

inline Vec3 operator* (const double s, const Vec3 &v) {
    return v * s;
}
inline Vec3 operator/ (const double s, const Vec3 &v) {
    return v / s;
}

// Does NOT normalize the given vector, just returns the unit vector.
inline Vec3 normalized(const Vec3 a) {
    return a / a.length();
}

inline double dot(const Vec3 &a, const Vec3 &b) {
    return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return {
	    a.e[1] * b.e[2] - a.e[2] * b.e[1],
        a.e[2] * b.e[0] - a.e[0] * b.e[2],
        a.e[0] * b.e[1] - a.e[1] * b.e[0]
    };
}

inline Vec3 random_in_unit_sphere() {
  while (true) {
    auto p = Vec3::random(-1, 1);
    if (p.length_squared() >= 1) continue;
    return p;
  }
}

inline Vec3 random_in_hemisphere(const Vec3 normal) {
    Vec3 in_unit_sphere = random_in_unit_sphere();

    if (dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    }

    return -in_unit_sphere;
}

inline Vec3 random_in_unit_disk()
{
	while(true) {
        auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);

        if (p.length_squared() >= 1) continue;

        return p;
	}
}

inline Vec3 random_unit_vector() {
  return normalized(random_in_unit_sphere());
}

inline Vec3 reflect(const Vec3 v, const Vec3 n) {
  return v - 2 * dot(v,n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat)
{
    auto cost_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cost_theta * n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

using Point3 = Vec3;
using Color = Vec3;

#endif