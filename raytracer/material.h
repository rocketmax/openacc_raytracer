//
//  File name: material.h
//
//  Authors: Created by Krzysztof Gabis on 25.03.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Definition for a material object that contains the spectral characteristics of a surface
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_material_h
#define raytracer_material_h

#include "color.h"

typedef struct {
    Color color;
    double reflectivity;
    double specularity;
    double reflectionNoise;
} Material;

Material material_make(Color color, double reflectivity, double reflectionNoise, double specularity);

#endif
