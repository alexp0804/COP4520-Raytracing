#ifndef VEC3
#define VEC3

#include <cmath>
#include <iostream>

#define ENTRIES 3

class Vec3 {
    // Vec3 fields
    public:
        double e[ENTRIES];

    // Vec3 methods
    public:
        Vec3() {
            for (int i = 0; i < ENTRIES; i++)
                e[i] = (double) 0;
        }
        Vec3(double x, double y, double z) {
            e[0] = x;
            e[1] = y;
            e[2] = z;
        }

        double x() { return e[0]; }
        double y() { return e[1]; }
        double z() { return e[2]; }

        double length_squared() {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
        double length() {
            return std::sqrt(length_squared());
        }

    // Vec3 Operators

    // Negate
    Vec3 operator- () const { return Vec3(-e[0], -e[1], -e[2]); }

    // Subscript
    double operator[] (int i) const { return e[i]; }
    double& operator[] (int i) { return e[i]; }

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
    inline Vec3 operator+ (const Vec3 &v) {
        return Vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
    }
    // Subtract
    inline Vec3 operator- (const Vec3 &v) {
        return Vec3(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
    }
    
    // Multiply (by scalar)
    inline Vec3 operator* (double s) {
        return Vec3(e[0] * s, e[1] * s, e[2] * s);
    }
    // Divide (by scalar)
    inline Vec3 operator/ (double s) {
        return Vec3(e[0] / s, e[1] / s, e[2] / s);
    }

    // Multiply (by vector, element-wise)
    inline Vec3 operator* (const Vec3 &v) {
        return Vec3(e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]);
    }

    // Print
    inline std::ostream& operator<< (std::ostream &out) {
        return out << e[0] << ' ' << e[1] << ' ' << e[2] << std::endl;
    }
};

// Does NOT normalize the given vector, just returns the unit vector.
inline Vec3 normalized(Vec3 a) {
    return a / a.length();
}

inline double dot(const Vec3 &a, const Vec3 &b) {
    return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        a.e[1] * b.e[2] - a.e[2] * b.e[1],
        a.e[2] * b.e[0] - a.e[0] * b.e[2],
        a.e[0] * b.e[1] - a.e[1] * b.e[0]
    );
}

using Point3 = Vec3;
using Color = Vec3;

#endif