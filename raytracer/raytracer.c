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
    //#pragma acc parallel num_gangs(rt->resolutionX * rt->resolutionY / 8) \
    //                     num_workers(8)
    //{
    //#pragma acc kernels
    //for (int x = 0; x < rt->resolutionX; x++) {
    //    for (int y = 0; y < rt->resolutionY; y++) {
    //        Ray ray = ray_makeForPixel(&rt->scene.camera, x, y);
    //        Color color = ray_trace(&ray, &rt->scene);
    //        draw(data, color, x, y);
    //    }
    //}
    //}
    Ray ray[X_TILE_WIDTH][Y_TILE_WIDTH];
    Color color[X_TILE_WIDTH][Y_TILE_WIDTH];    
    
    for (int tileY = 0; tileY < rt->resolutionY; tileY += Y_TILE_WIDTH) {
        for(int tileX = 0; tileX < rt->resolutionX; tileX += X_TILE_WIDTH) {
    //int tileY = 0; int tileX = 0;
    
            //printf("make %d, %d\r\n", tileX, tileY);
            //clock_t before = clock();
            for (int y = 0; y < Y_TILE_WIDTH; y++) {
                for (int x = 0; x < X_TILE_WIDTH; x++) {
                    if(tileX + x < rt->resolutionX && tileY + y < rt->resolutionY)
                        ray[x][y] = ray_makeForPixel(&rt->scene.camera, tileX + x, tileY + y); 
                }
            }
            //clock_t difference = clock() - before;
            //float sec = (float) difference / CLOCKS_PER_SEC;
            //printf("Time taken: %f\r\n", sec);
        
            //before = clock();   
        
            //printf("trace %d, %d\r\n", tileX, tileY);
            #pragma acc parallel vector_length(128) \
                copyin(rt[0:1])   \
                create(rt->scene) 
            {
                
            #pragma acc loop independent 
            for (int y = 0; y < Y_TILE_WIDTH; y++) {
                #pragma acc loop independent
                for (int x = 0; x < X_TILE_WIDTH; x++) {
                    if(tileX + x < rt->resolutionX && tileY + y < rt->resolutionY){
                        color[x][y] = ray_trace(&ray[x][y], &rt->scene); printf("x: %d, y: %d, tilex %d, tiley%d\r\n", x, y, tileX, tileY); }
                }
            }
            }
            //difference = clock() - before;
            //sec = (float) difference / CLOCKS_PER_SEC;
            //printf("Time taken: %f\r\n", sec);
                
            //before = clock();
            //printf("draw %d, %d\r\n", tileX, tileY);
            for (int y = 0; y < Y_TILE_WIDTH; y++) {
                for (int x = 0; x < X_TILE_WIDTH; x++) {
                    if(tileX + x < rt->resolutionX && tileY + y < rt->resolutionY)
                        draw(data, color[x][y], tileX + x, tileY + y);
                }
            }
            //difference = clock() - before;
            //sec = (float) difference / CLOCKS_PER_SEC;
            //printf("Time taken: %f\r\n", sec);
            //printf("%d/%d\r\n", y, rt->resolutionY);
        }
    }
}

void raytracer_dealloc(Raytracer *rt) {
    scene_dealloc(&rt->scene);
}
