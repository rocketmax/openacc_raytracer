//
//  File name: material.c
//
//  Authors: Created by Krzysztof Gabis on 31.03.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: An initializer function for creating a material object 
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "material.h"

Material material_make(Color color, double reflectivity, double reflectionNoise, double specularity) {
    Material m;
    m.color = color;
    m.reflectivity = reflectivity;
    m.specularity = specularity;
    m.reflectionNoise = reflectionNoise;
    return m;
}