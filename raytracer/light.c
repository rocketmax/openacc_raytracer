//
//  File name: light.c
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013 
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Functions using concepts of light dispersion to simulate light in scene. This includes direction, position, 
//                 intensity, highlights, shadows, etc. These concepts can be referred to here:
//                 http://en.wikipedia.org/wiki/Phong_reflection_model  
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//


#include <math.h>
#include "light.h"

// Specular reflection constant
#define KS_CONST 1.0

Light light_make(Vector3 position, double intensity) {
    Light l = { .position = position, .intensity = intensity };
    return l;
}

Vector3 light_getDirection(Vector3 position, Vector3 point) {
    return vec3_unit(vec3_sub(point, position));
}

double light_getDiffusedHighlight(const Light *light, Vector3 direction, Vector3 normal) {
    double highlight = VEC3_DOT(normal, direction);
    if (highlight < 0) {
        return 0;
    }
    return highlight * light->intensity;
}

double light_getSpecularHighlight(const Light *light, Vector3 lightDirection, Vector3 normal, Vector3 rayDirection, double specularity) {
    double highlight = VEC3_DOT(normal, lightDirection);
    Vector3 V = vec3_negate(rayDirection);
    Vector3 R = vec3_sub(lightDirection, vec3_mult(normal, highlight * 2.0));
    double dot = vec3_dot(V, R);
    if (dot < 0) {
        return 0;
    }
    double spec = pow(dot, specularity) * KS_CONST * light->intensity;
    return spec;
}
