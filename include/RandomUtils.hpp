#pragma once

#include <algorithm>
#include <cstdint>
#include <random>

const uint64_t N = 6364136223846793005ULL;
const double one_over_max = 0x1.0p-32;

inline uint64_t
get_seed()
{
    std::random_device rd;
    uint64_t seed = (static_cast<uint64_t>(rd()) << 32) | rd();
    return seed;
}
inline uint64_t get_initial_state(uint64_t pcg_seed)
{
    uint64_t inc = (pcg_seed << 1) | 1;
    return (inc + pcg_seed) * N + inc;
}

inline thread_local uint64_t pcg_seed = get_seed();
inline thread_local uint64_t inc = (pcg_seed << 1) | 1;
inline thread_local uint64_t pcg_state = get_initial_state(pcg_seed);

// https://rosettacode.org/wiki/Pseudo-random_numbers/PCG32
inline uint32_t pcg32_random()
{
    uint64_t old_state = pcg_state;

    pcg_state = old_state * N + inc;

    uint32_t xs = static_cast<uint32_t>(((old_state >> 18u) ^ old_state) >> 27u);
    uint32_t rot = static_cast<uint32_t>(old_state >> 59u);

    return (xs >> rot) | (xs << ((-rot) & 31));
}

inline double get_zero_to_one()
{
    return ((double)pcg32_random()) * one_over_max;
}

inline double get_random_double(double min, double max)
{
    return min + (max - min) * get_zero_to_one();
}