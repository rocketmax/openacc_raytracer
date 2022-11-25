//
//  File name: main.c 
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013 
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Main function that serves as the starting point for program execution and controls program flow.
//                 It gives the user the chance to select which scene to render and renders it using bitmaps or SFML library.
//                 It intializes and calls the raytracer function which does the main work of rendering 
//                 There is also has timing module to measure time taken.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "raytracer.h"
#include "csfml_render.h"
#include "png_render.h"

#include "utils.h"

// Output image resolution
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void selectDemo(int argc, const char *argv[], Scene *scene) {
    if (argc != 2) {
        printf("Usage: %s DEMO_NAME\n", argv[0]);
        printf("Available demos: spheres, snowman, teapot\n");
        scene_loadTeapotDemo(scene);
    } else if (STREQ(argv[1], "spheres")) {
        scene_loadSpheresDemo(scene);
    } else if (STREQ(argv[1], "snowman")) {
        scene_loadSnowmanDemo(scene);
    } else if (STREQ(argv[1], "teapot")) {
        scene_loadTeapotDemo(scene);
    }
}

int main(int argc, const char *argv[]) {

    clock_t before = clock();
    
    Raytracer rt;
    raytracer_init(&rt, WINDOW_WIDTH, WINDOW_HEIGHT);
    selectDemo(argc, argv, &rt.scene);   
#ifdef PNG
    png_render(&rt);
#else
    csfml_render(&rt);
#endif
    raytracer_dealloc(&rt);
    
    clock_t difference = clock() - before;
    float sec = (float) difference / CLOCKS_PER_SEC;
    printf("Time taken: %f\r\n", sec);
    
    return 0;    
}
