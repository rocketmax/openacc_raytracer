//
//  File name: raytracer.h
//
//  Authors: Created by Krzysztof Gabis on 25.03.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Defines Raytracer struct to hold the image resolution and scene.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_raytracer_h
#define raytracer_raytracer_h

#include <stdlib.h>
#include "geometry.h"
#include "scene.h"

typedef void (*DrawFunction)(void *data, Color color, size_t x, size_t y);

typedef struct {
    Scene scene;
    size_t resolutionX;
    size_t resolutionY;
} Raytracer;

void raytracer_init(Raytracer *rt, size_t resolutionX, size_t resolutionY);
void raytracer_render(Raytracer *rt, DrawFunction draw, void *data);
void raytracer_dealloc(Raytracer *rt);

#endif
