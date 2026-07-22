#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

constexpr double ZERO_TOLERANCE = 1e-5;

struct Interval
{
public:
    double min;
    double max;

    static constexpr double universe_min() { return std::numeric_limits<double>::lowest(); }
    static constexpr double universe_max() { return std::numeric_limits<double>::max(); }

    Interval() : min(universe_max()), max(universe_min()) {} // The default constructor builds an empty interval

    Interval(double _min, double _max) : min(_min), max(_max) {}

    Interval(const std::vector<double> &values)
    {
        min = universe_max();
        max = universe_min();
        for (double value : values)
        {
            min = std::min(min, value);
            max = std::max(max, value);
        }
    }

    inline static Interval empty()
    {
        return Interval(universe_max(), universe_min());
    }

    inline static Interval universe()
    {
        return Interval(universe_min(), universe_max());
    }

    inline bool is_empty() const
    {
        return min > max;
    }

    inline bool overlaps(const Interval &other) const
    {
        if (is_empty() || other.is_empty())
        {
            return false;
        }

        double overlap_start = std::max(min, other.min);
        double overlap_end = std::min(max, other.max);
        return overlap_start < overlap_end;
    }

    inline Interval intersect_with(const Interval &other) const
    {
        return Interval(std::max(min, other.min), std::min(max, other.max));
    }

    inline Interval expand(double delta) const
    {
        if (is_empty())
        {
            return empty();
        }
        const double new_min = (min == universe_min()) ? min : min - delta / 2.0;
        const double new_max = (max == universe_max()) ? max : max + delta / 2.0;
        return Interval(new_min, new_max);
    }

    inline Interval extend_to_include(double value) const
    {
        return Interval(std::min(min, value), std::max(max, value));
    }

    inline Interval union_with(const Interval &other) const
    {
        return Interval(std::min(min, other.min), std::max(max, other.max));
    }

    inline bool contains_value(double x) const
    {
        return min <= x && x <= max;
    }

    inline Interval ray_intersect(double start_pos, double delta_pos) const
    {
        if (is_empty())
        {
            return empty();
        }
        if (std::abs(delta_pos) < ZERO_TOLERANCE)
        {
            if (contains_value(start_pos))
            {
                return universe();
            }
            else
            {
                return empty();
            }
        }

        double t0 = (min - start_pos) / delta_pos;
        double t1 = (max - start_pos) / delta_pos;

        if (t0 > t1)
        {
            std::swap(t0, t1);
        }

        return Interval(t0, t1);
    }
};