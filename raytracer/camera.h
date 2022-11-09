//
//  File name: camera.h
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013 
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Data structure for camera needed in scene creation (parameters in this use the Vector structure), and function 
//                 declarations for "camera.h"
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_camera_h
#define raytracer_camera_h

#include "vector.h"

typedef struct {
    Vector3 position;
    Vector3 direction;
    Vector3 planeCenter;
    Vector3 planeDirectionX;
    Vector3 planeDirectionY;
    double focalLength;
    double width;
    double height;
} Camera;

void camera_init(Camera *cam, Vector3 position, Vector3 direction, double focalLength, double width, double height);
void camera_setup(Camera *c);

#endif
