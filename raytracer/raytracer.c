//
//  raytracer.c
//  raytracer
//
//  Created by Krzysztof Gabis on 29.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
#include <stdio.h>

#include "raytracer.h"
#include "ray.h"

void raytracer_init(Raytracer *rt, size_t resolutionX, size_t resolutionY) {
    scene_init(&rt->scene);
    rt->resolutionX = resolutionX;
    rt->resolutionY = resolutionY;
}

void raytracer_loadDemo(Raytracer *rt) {
    scene_loadDemo(&rt->scene);
}

void raytracer_render(Raytracer *rt, DrawFunction draw, void *data) {
//    Light *light = array_get(&rt->scene.lights, 0);
//    light_moveLeftRight(light);
    camera_setup(&rt->scene.camera);
    for (size_t x = 0; x < rt->resolutionX; x++) {
        for (size_t y = 0; y < rt->resolutionY; y++) {
            Ray ray = ray_makeForPixel(&rt->scene.camera, x, y);
            Color color = ray_trace(ray, &rt->scene);
            draw(data, color, x, y);
        }
    }
}

void raytracer_dealloc(Raytracer *rt) {
    scene_dealloc(&rt->scene);
}
