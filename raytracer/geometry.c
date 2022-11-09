//
//  File name: geometry.c
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013 
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Intialization functions for the geometric shapes present in the possible scenes. 
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>

#include "geometry.h"

Sphere sphere_make(Vector3 center, double radius) {
    Sphere s = { .center = center, .radius = radius };
    return s;
}

Triangle triangle_make(Vector3 a, Vector3 b, Vector3 c) {
    Triangle t = { .a = a, .b = b, .c = c };
    VEC3_SUB(t.edges[0], t.b, t.a);
    VEC3_SUB(t.edges[1], t.c, t.a);
    t.normal = vec3_unit(vec3_cross(vec3_sub(t.b, t.a), vec3_sub(t.c, t.a)));    
    return t;
}
