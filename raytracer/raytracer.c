//
//  raytracer.c
//  raytracer
//
//  Created by Krzysztof Gabis on 29.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
// 17.08.2013 15:10 - O3 
//Teapot:
//Seconds per frame: 97.572769
// 17.08.2013 15:37 - O3 OMP 
//Teapot:
//Seconds per frame: 54.484337
//Seconds per frame: 54.581848
//

#include <stdio.h>

#include <time.h>

#include "raytracer.h"
#include "ray.h"

#define Y_TILE_WIDTH 128 
#define X_TILE_WIDTH 128

void raytracer_init(Raytracer *rt, size_t resolutionX, size_t resolutionY) {
    scene_init(&rt->scene, resolutionX, resolutionY);
    rt->resolutionX = resolutionX;
    rt->resolutionY = resolutionY;
}

void raytracer_render(Raytracer *rt, DrawFunction draw, void *data) {
    camera_setup(&rt->scene.camera);
    Ray ray[X_TILE_WIDTH][Y_TILE_WIDTH];
    Color color[X_TILE_WIDTH][Y_TILE_WIDTH];    
    

    for (int tileY = 0; tileY < rt->resolutionY; tileY += Y_TILE_WIDTH) {
        for(int tileX = 0; tileX < rt->resolutionX; tileX += X_TILE_WIDTH) {
    
            for (int y = 0; y < Y_TILE_WIDTH; y++) {
                for (int x = 0; x < X_TILE_WIDTH; x++) {
                    if(tileX + x < rt->resolutionX && tileY + y < rt->resolutionY)
                        ray[x][y] = ray_makeForPixel(&rt->scene.camera, tileX + x, tileY + y); 
                }
            }
            #pragma acc kernels vector_length(16) \
                          copyin(rt[0:1])   \
                          copyin(rt->scene)  \
                          copyin(ray[0:X_TILE_WIDTH][0:Y_TILE_WIDTH]) \
                          create(color[0:X_TILE_WIDTH][0:Y_TILE_WIDTH])  \
                          copyout(color[0:X_TILE_WIDTH][0:Y_TILE_WIDTH]) 
            {       
            #pragma acc loop independent 
            for (int y = 0; y < Y_TILE_WIDTH; y++) {
                #pragma acc loop independent
                for (int x = 0; x < X_TILE_WIDTH; x++) {
                    if(tileX + x < rt->resolutionX && tileY + y < rt->resolutionY)
                        color[x][y] = ray_trace(&ray[x][y], &rt->scene); 
                }
            }
            }

            for (int y = 0; y < Y_TILE_WIDTH; y++) {
                for (int x = 0; x < X_TILE_WIDTH; x++) {
                    if(tileX + x < rt->resolutionX && tileY + y < rt->resolutionY)
                        draw(data, color[x][y], tileX + x, tileY + y);
                }
            }
            
        }
    }
}

void raytracer_dealloc(Raytracer *rt) {
    scene_dealloc(&rt->scene);
}
