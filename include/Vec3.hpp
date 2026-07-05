#pragma once

#include <cmath>
#include <numbers>

#include "RandomUtils.hpp"

const double ZERO_TOLERANCE = 0.001;

struct Vec3
{
public:
    double x, y, z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
    Vec3(double v) : x(v), y(v), z(v) {}

    inline Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    inline Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    inline Vec3 operator*(const Vec3 &other) const
    {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    inline Vec3 operator/(const Vec3 &other) const
    {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }

    inline Vec3 operator+(const double n) const
    {
        return Vec3(x + n, y + n, z + n);
    }

    inline Vec3 operator-(const double n) const
    {
        return Vec3(x - n, y - n, z - n);
    }

    inline Vec3 operator*(double n) const
    {
        return Vec3(n * x, n * y, n * z);
    }

    inline Vec3 operator/(double n) const
    {
        return Vec3(x / n, y / n, z / n);
    }

    inline double dot(const Vec3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    // returns "this cross other"
    inline Vec3 cross(const Vec3 &other) const
    {
        return Vec3(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
    }

    inline double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline double length_squared() const
    {
        return x * x + y * y + z * z;
    }

    inline Vec3 normalise() const
    {
        double length = this->length();
        return Vec3(x / length, y / length, z / length);
    }

    inline Vec3 sqrt() const
    {
        return Vec3(std::sqrt(x), std::sqrt(y), std::sqrt(z));
    }

    // Takes in a normalised normal to the plane the current vector hits and reflects the incoming vector in this plane
    inline Vec3 reflect(const Vec3 &normal) const
    {
        return *this - (normal * (normal.dot(*this) * 2.0));
    }

    // Takes in a normalised normal to the plane the current vector hits and reflects the incoming normalised vector in this plane. The vector is assumed to be pointing into the plane, so this.dot(normal) <= 0
    inline Vec3 refract(const Vec3 &normal, double n_previous_over_n_next) const
    {
        double minus_cos_theta = this->dot(normal);
        Vec3 d_perp = (*this - normal * minus_cos_theta) * n_previous_over_n_next;
        double par_squared = std::fmax(0.0, 1.0 - d_perp.length_squared());
        Vec3 d_par = normal * (-std::sqrt(par_squared));
        Vec3 refracted_direction = d_perp + d_par;
        return refracted_direction;
    }

    static Vec3 random_unit_vector()
    {
        double z = get_random_double(-1.0, 1.0);
        double r_at_altitude = std::sqrt(1.0 - z * z);
        double lambda = get_random_double(0.0, 1.0) * 2.0 * std::numbers::pi_v<double>;
        double x = std::cos(lambda) * r_at_altitude;
        double y = std::sin(lambda) * r_at_altitude;
        return Vec3(x, y, z);
    }

    Vec3 random_vector_in_hemisphere_around() const
    {
        Vec3 random_vec = Vec3::random_unit_vector();
        if (random_vec.dot(*this) < 0.0)
        {
            return random_vec * (-1.0);
        }
        return random_vec;
    }

    inline bool is_close_to_zero() const
    {
        return this->length_squared() < ZERO_TOLERANCE * ZERO_TOLERANCE;
    }
};