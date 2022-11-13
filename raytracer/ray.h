//
//  File name: ray.h
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Defines the Ray object, composed of an origin and a direction
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_ray_h
#define raytracer_ray_h

#include "vector.h"
#include "color.h"
#include "surface.h"
#include "scene.h"
#include "camera.h"

typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

Ray ray_make(Vector3 origin, Vector3 direction);
Ray ray_makeForPixel(const Camera *c, size_t x, size_t y);
#pragma acc routine seq
Color ray_trace(const Ray *ray, const Scene *scene);

#endif
