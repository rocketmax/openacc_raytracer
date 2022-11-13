//
//  File name: surface.c
//
//  Authors: Created by Krzysztof Gabis on 30.03.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Helps with initializing sphere and triangle objects, as well as getting the normal vector for points on the surface.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "surface.h"

Surface surface_initSphere(Vector3 position, double radius, Material material) {
    Surface s = { .type = GTSphere,
                  .geometry.sphere = sphere_make(position, radius),
                  .material = material };
    return s;
}

Surface surface_initTriangle(Vector3 a, Vector3 b, Vector3 c, Material material) {
    Surface t = { .type = GTTriangle,
                  .geometry.triangle = triangle_make(a, b, c),
                  .material = material };
    return t;
}

Vector3 surface_getNormalAtPoint(const Surface *surface, Vector3 point) {
    switch (surface->type) {
        case GTTriangle:
            return surface->geometry.triangle.normal;
        case GTSphere:
            return vec3_unit(vec3_sub(surface->geometry.sphere.center, point));
        default:
            return VEC3_ZERO;
    }
}