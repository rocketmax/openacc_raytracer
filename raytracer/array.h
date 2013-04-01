//
//  array.h
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_array_h
#define raytracer_array_h

typedef struct {
    void *items;
    size_t itemSize;
    size_t count;
    size_t capacity;
} Array;

Array *array_init(Array *array, size_t itemSize, size_t initialCapacity);
int array_add(Array *array, void *item); // 1 on success
void* array_get(Array *array, size_t index);
size_t array_getCount(Array *array);
void array_remove(Array *array, size_t index);
void array_dealloc(Array *array);

#endif
