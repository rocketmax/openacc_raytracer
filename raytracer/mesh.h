//
//  File name: mesh.h
//
//  Authors: Created by Krzysztof Gabis on 08.04.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: The definition of a mesh object (unused)
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_mesh_h
#define raytracer_mesh_h

#include "array.h"
#include "material.h"
#include "geometry.h"

typedef struct {
    Array faces;
    Sphere boundingVolume;
} Mesh;

Array mesh_load(const char *filename, Material material);

#endif
