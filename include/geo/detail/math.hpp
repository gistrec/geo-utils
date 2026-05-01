// Copyright 2026 Aleksandr Kovalko
// Licensed under the Apache License, Version 2.0
//
// Portions of this file are based on Google Maps Android Utils:
// https://github.com/googlemaps/android-maps-utils
//
// Original work:
// Copyright 2013 Google Inc.
// Licensed under the Apache License, Version 2.0
//
// This file has been modified from the original work,
// including a port from Java to C++.

#pragma once

#include <cmath>

namespace geo::detail {

// Mean Earth radius (IUGG), in meters.
inline constexpr double kEarthRadius = 6371009.0;

// We define our own constant rather than rely on M_PI (POSIX extension,
// not portable to MSVC without _USE_MATH_DEFINES) or std::numbers::pi (C++20).
inline constexpr double kPi = 3.14159265358979323846;

inline double deg2rad(double degrees) noexcept {
    return degrees * kPi / 180.0;
}

inline double rad2deg(double angle) noexcept {
    return angle * 180.0 / kPi;
}

// Restrict x to the range [low, high].
inline double clamp(double x, double low, double high) noexcept {
    return x < low ? low : (x > high ? high : x);
}

// Returns the non-negative remainder of x / m.
inline double mod(double x, double m) noexcept {
    return std::fmod(std::fmod(x, m) + m, m);
}

// Wraps the given value into the inclusive-exclusive interval [min, max).
inline double wrap(double n, double min, double max) noexcept {
    return (n >= min && n < max) ? n : (mod(n - min, max - min) + min);
}

// Returns mercator Y corresponding to latitude.
inline double mercator(double lat) noexcept {
    return std::log(std::tan(lat * 0.5 + kPi / 4.0));
}

// Returns latitude from mercator Y.
inline double inverse_mercator(double y) noexcept {
    return 2.0 * std::atan(std::exp(y)) - kPi / 2.0;
}

// Returns haversine(angle-in-radians).
// hav(x) == (1 - cos(x)) / 2 == sin(x / 2)^2.
inline double hav(double x) noexcept {
    double sin_half = std::sin(x * 0.5);
    return sin_half * sin_half;
}

// Inverse haversine. arc_hav(x) == 2 * asin(sqrt(x)). Argument must be in [0, 1].
inline double arc_hav(double x) noexcept {
    return 2.0 * std::asin(std::sqrt(clamp(x, 0.0, 1.0)));
}

// Given h == hav(x), returns sin(abs(x)).
inline double sin_from_hav(double h) noexcept {
    return 2.0 * std::sqrt(h * (1.0 - h));
}

// Returns hav(asin(x)).
inline double hav_from_sin(double x) noexcept {
    double x2 = x * x;
    return x2 / (1.0 + std::sqrt(1.0 - x2)) * 0.5;
}

// Returns sin(arc_hav(x) + arc_hav(y)).
inline double sin_sum_from_hav(double x, double y) noexcept {
    double a = std::sqrt(x * (1.0 - x));
    double b = std::sqrt(y * (1.0 - y));
    return 2.0 * (a + b - 2.0 * (a * y + b * x));
}

// Returns hav() of distance from (lat1, lng1) to (lat2, lng2) on the unit sphere.
inline double hav_distance(double lat1, double lat2, double d_lng) noexcept {
    return hav(lat1 - lat2) + hav(d_lng) * std::cos(lat1) * std::cos(lat2);
}

}  // namespace geo::detail
