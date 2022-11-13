//
//  File name: utils.h
//
//  Authors: Created by Krzysztof Gabis on 06.04.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Precompiler math helper functions
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_utils_h
#define raytracer_utils_h

#define SQUARE(x) ((x)*(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define LIMIT(val, min, max) ((val) > (max) ? (max) : ((val) < (min) ? (min) : (val)))
#define STREQ(a,b) (strcmp(a,b) == 0)
#endif
