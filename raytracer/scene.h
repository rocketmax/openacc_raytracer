//
//  File name: scene.c
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Defines the scene object to hold surfaces, lights, the camera, background color, and ambient coefficient.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_scene_h
#define raytracer_scene_h

#include "array.h"
#include "camera.h"
#include "color.h"
#include "surface.h"
#include "light.h"

typedef struct {
    Array surfaces;
    Array lights;
    double ambientCoefficient;
    Camera camera;
    Color backgroundColor;
} Scene;

void scene_init(Scene *scene, size_t width, size_t height);
void scene_loadSpheresDemo(Scene *scene);
void scene_loadTeapotDemo(Scene *scene);
void scene_loadSnowmanDemo(Scene *scene);
void scene_loadMesh(Scene *scene, const char *filename, Material meshMaterial);
void scene_AddLight(Scene *scene, Light *light);
void scene_AddLightRange(Scene *scene, Light *lights, size_t len);
void scene_addSurface(Scene *scene, Surface *surface);
void scene_addSurfaceRange(Scene *scene, Surface *surfaces, size_t len);
void scene_dealloc(Scene *scene);

#endif
