#pragma once

#include <cmath>
#include <numbers>

#include "RandomUtils.hpp"

struct Vec3
{
public:
    double x, y, z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    inline Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    inline Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
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

    inline double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline Vec3 normalise() const
    {
        double length = this->length();
        return Vec3(x / length, y / length, z / length);
    }

    // Takes in a normalised normal to the plane the current vector hits and reflects the incoming vector in this plane
    inline Vec3 reflect(const Vec3 &normal) const
    {
        return *this - (normal * (normal.dot(*this) * 2.0));
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
};