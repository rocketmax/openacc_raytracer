//
//  File name: light.h
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013 
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Data structure for light structure and function declarations for file "color.c". Note that this structure uses 
//                 vector data structure. This file also contains routine pragma for the functions,
//                 that tells it how it should be called. As there was little opportunity for parallelization in
//                 these functions, they were set to “seq”, which specifies that the function should run sequentially.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_light_h
#define raytracer_light_h

#include "vector.h"
#include "color.h"

typedef struct {
    Vector3 position;
    double intensity;
} Light;

#pragma acc routine seq
Light light_make(Vector3 position, double intensity);
#pragma acc routine seq
Vector3 light_getDirection(Vector3 position, Vector3 point);
#pragma acc routine seq
double light_getDiffusedHighlight(const Light *light, Vector3 lightDirection, Vector3 normal);
#pragma acc routine seq
double light_getSpecularHighlight(const Light *light, Vector3 lightDirection,
                                  Vector3 normal, Vector3 rayDirection, double specularity);

#endif
