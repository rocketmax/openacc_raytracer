//
//  File name: color.h
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013 
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Data structure for Color structure (RBG format), needed constants for hex conversion and function 
//                 declarations for file "color.c". This file also contains routine pragma for the functions,
//                 that tells it how it should be called. As there was little opportunity for parallelization in many
//                 of these functions, they were set to “seq”, which specifies that the function should run sequentially.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_color_h
#define raytracer_color_h

#define COLOR_BLACK color_makeFromRGBhex(0x000000)
#define COLOR_WHITE color_makeFromRGBhex(0xFFFFFF)
#define COLOR_RED   color_makeFromRGBhex(0xFF0000)
#define COLOR_GREEN color_makeFromRGBhex(0x00FF00)
#define COLOR_BLUE  color_makeFromRGBhex(0x0000FF)

// http://www.colourlovers.com/palette/433018/Gasoline_Rainbow
#define COLORS1_RED    color_makeFromRGB(189, 42, 51)
#define COLORS1_YELLOW color_makeFromRGB(214, 170, 38)
#define COLORS1_GREEN  color_makeFromRGB(64, 129, 86)
#define COLORS1_GREEN2 color_makeFromRGB(147, 163, 28)
#define COLORS1_BLUE   color_makeFromRGB(48, 55, 79)

// http://www.colourlovers.com/palette/412963/Retro_Rainbow
#define COLORS2_RED    color_makeFromRGBhex(0xC00F12)
#define COLORS2_ORANGE color_makeFromRGBhex(0xD27701)
#define COLORS2_YELLOW  color_makeFromRGBhex(0xDDBA01)
#define COLORS2_GREEN color_makeFromRGBhex(0x007A28)
#define COLORS2_BLUE   color_makeFromRGBhex(0x3B9B95)

typedef struct {
    double r, g, b;
} Color;

#pragma acc routine seq
Color color_make(double r, double g, double b);
#pragma acc routine seq
Color color_makeFromRGB(unsigned char r, unsigned char g, unsigned char b);
#pragma acc routine seq
Color color_makeFromRGBhex(unsigned int c);
#pragma acc routine seq
Color color_add(Color a, Color b);
#pragma acc routine seq
Color color_addWeighted(Color a, double weightA, Color b, double weightB);
#pragma acc routine seq
Color color_mult(Color c, double n);
#pragma acc routine seq
Color color_blend(Color a, double weightA, Color b);

#endif
