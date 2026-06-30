#pragma once

#include <random>

inline double get_zero_to_one()
{
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());
    thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);

    return dist(gen);
}

inline double get_random_double(double min, double max)
{
    return min + (max - min) * get_zero_to_one();
}