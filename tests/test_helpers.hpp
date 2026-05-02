#pragma once
#include <cmath>
#include <gtest/gtest.h>
#include <geo/detail/math.hpp>

// Checks that two LatLng values agree within 1e-6. Longitude is weighted by
// cos(lat) so the assertion degenerates gracefully at poles where longitude is
// geometrically undefined. Both longitudes are wrapped to (-180, 180] first so
// that -180 and +180 (the same antimeridian point) compare as equal.
#define EXPECT_NEAR_LatLng(expected, actual)                                          \
    do {                                                                              \
        EXPECT_NEAR((expected).lat, (actual).lat, 1e-6);                              \
        const double _cosLat = std::cos(::geo::detail::deg2rad((expected).lat));      \
        const double _eLng = ::geo::detail::wrap((expected).lng, -180.0, 180.0);      \
        const double _aLng = ::geo::detail::wrap((actual).lng,   -180.0, 180.0);      \
        EXPECT_NEAR(_cosLat * _eLng, _cosLat * _aLng, 1e-6);                          \
    } while (false)
