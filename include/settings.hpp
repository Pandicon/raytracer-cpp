#pragma once

#include <optional>
#include <string_view>

namespace Settings
{
    // Toggle to true for discrete RGB lasers (420nm, 532nm, 650nm) - useful for testing for faster noise convergence, but not really physically accurate (only a rough approximation) both in terms of colour and in terms of per-wavelength behaviour
    // Toggle to false for continuous wavelengths (MIN_LAMBDA_NM - MAX_LAMBDA_NM, defined in limits.hpp)
    constexpr bool USE_DISCRETE_WAVELENGTHS = false;

    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 800;
    constexpr double FRAMES_PER_LOOP = 10.0;
    constexpr std::optional<std::string_view> FILE_TO_LOAD = "1786223751";
    constexpr std::optional<unsigned int> THREADS_TO_USE_OVERRIDE = std::nullopt;
}