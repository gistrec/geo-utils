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

#ifndef GEOMETRY_LIBRARY_LATLNG
#define GEOMETRY_LIBRARY_LATLNG

#include <cmath>

class LatLng {
public:
    double lat; // The latitude  of this location
    double lng; // The longitude of this location

    /**
     * Constructs a location with a latitude/longitude pair.
     *
     * @param lat   The latitude  of this location.
     * @param lng   The longitude of this location.
     */
    LatLng(double lat, double lng)
        : lat(lat), lng(lng) {}

    LatLng(const LatLng & point) = default;

    LatLng& operator=(const LatLng & other) = default;

    bool operator==(const LatLng & other) const {
        return isCoordinateEqual(lat, other.lat) &&
               isLngEqual(lng, other.lng);
    }


private:
    bool isCoordinateEqual(double first, double second) const {
        return std::fabs(first - second) < 1e-12;
    }

    // Longitude wraps: 180° and -180° are the same meridian.
    bool isLngEqual(double a, double b) const {
        double diff = std::fabs(std::fmod(a - b, 360.0));
        if (diff > 180.0) diff = 360.0 - diff;
        return diff < 1e-12;
    }
};

#endif // GEOMETRY_LIBRARY_LATLNG
