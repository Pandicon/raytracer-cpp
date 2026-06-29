#pragma once

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
        return Vec3(n / x, n / y, n / z);
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
};