//
//  File name: ray.c
//
//  Authors: Created by Krzysztof Gabis on 01.04.2013.
//           Modified by CSI 5640 Team (Max Theisen, Sumati Gupta, Prajakta Jadhav, Priya Kharde, Oluwakemi Ibrahim) on 11.09.2022
//
//  File contains: Contains the key functions that perform the tracing of the ray and calculate the result color accordingly.
//
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ray.h"
#include "utils.h"


#include <time.h>

#define MAX_RECURSION_DEPTH 4
#define MAX_VISIBLE_DISTANCE 600.0

#define EPSILON 0.000001

typedef struct {
    const Surface *surface;
    double distance;
} TracingResult;

typedef struct {
    double diffused;
    double specular;
} ShadingResult;

static Ray ray_addNoise(const Ray *ray, double magnitude);
static Ray ray_reflect(const Ray *ray, const Surface *surface, Vector3 point);
static Color ray_traceRecursive(const Ray *ray, const Scene *scene, size_t depth);
static TracingResult ray_traceOnce(const Ray *ray, const Scene *scene);
static int ray_checkIntersection(const Ray *ray, const Surface *surface, double *distance);
static int ray_checkSphereIntersection(const Ray *ray, const Sphere *sphere, double *distance);
static int ray_checkTriangleIntersection(const Ray *ray, const Triangle *triangle, double *distance);

static ShadingResult ray_shadeAtPoint(const Ray *ray, const Scene *scene, const Surface *surface, Vector3 point);
static Color getHighlightedColor(Color color, ShadingResult highlight, double ambientCoef);

Ray ray_make(Vector3 origin, Vector3 direction) {
    Ray r = { .origin = origin, .direction = direction};
    return r;
}

Ray ray_makeForPixel(const Camera *c, size_t x, size_t y) {
    double dy = 1.0;
    double dx = 1.0;
    double py = (- c->height / 2.0) + dy * ((double)y + 0.5);
    double px = (- c->width / 2.0) + dx * ((double)x + 0.5);
    Vector3 p = vec3_add3(c->planeCenter,
                          vec3_mult(c->planeDirectionX, px),
                          vec3_mult(c->planeDirectionY, py));
    Vector3 u_r = vec3_unit(vec3_sub(p, c->position));
    return ray_make(c->position, u_r);
}

Color ray_trace(const Ray *ray, const Scene *scene) {
    return ray_traceRecursive(ray, scene, MAX_RECURSION_DEPTH);
}

// Original ray_traceRecursive function that was not OpenACC safe

// static Color ray_traceRecursive(const Ray *ray, const Scene *scene, size_t depth) {
//     TracingResult closestHit = ray_traceOnce(ray, scene);
//     Color resultColor;
//     if (closestHit.surface == NULL) {
//         return scene->backgroundColor;
//     }
//     resultColor = closestHit.surface->material.color;
//     Vector3 collisionPoint = vec3_add(vec3_mult(ray->direction, closestHit.distance), ray->origin);
//     Material material = closestHit.surface->material;
//     if (material.reflectivity > 0.0 && depth > 0) {
//         Ray reflectedRay = ray_reflect(ray, closestHit.surface, collisionPoint);
//         if (material.reflectionNoise > 0) {
//             reflectedRay = ray_addNoise(&reflectedRay, material.reflectionNoise);                // This function was removed from inlined function
//         }
//         Color reflectionColor = ray_traceRecursive(&reflectedRay, scene, depth - 1);
//         resultColor = color_blend(reflectionColor, material.reflectivity, resultColor);
//     }
//     ShadingResult shadingResult = ray_shadeAtPoint(ray, scene, closestHit.surface, collisionPoint);    
//     resultColor = getHighlightedColor(resultColor, shadingResult, scene->ambientCoefficient);
//     resultColor = color_mult(resultColor, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
//     return resultColor;
// }

// Inlined version of the recursive function that is OpenACC safe
static Color ray_traceRecursive(const Ray *ray, const Scene *scene, size_t depth) {
    TracingResult closestHit = ray_traceOnce(ray, scene);
    Color resultColor;
    if (closestHit.surface == NULL) {
        
        return scene->backgroundColor;
    }
    resultColor = closestHit.surface->material.color;
    Vector3 collisionPoint = vec3_add(vec3_mult(ray->direction, closestHit.distance), ray->origin);
    Material material = closestHit.surface->material;
    if (material.reflectivity > 0.0 && depth > 0) {
        Ray reflectedRay = ray_reflect(ray, closestHit.surface, collisionPoint);
        Color reflectionColor;
        // Recurse level 1
        TracingResult closestHit1 = ray_traceOnce(&reflectedRay, scene);
        Color resultColor1;
        if (closestHit1.surface == NULL) {
            reflectionColor = scene->backgroundColor;
        }
        else
        {
          resultColor1 = closestHit1.surface->material.color;
          Vector3 collisionPoint1 = vec3_add(vec3_mult(reflectedRay.direction, closestHit1.distance), reflectedRay.origin);
          Material material1 = closestHit1.surface->material;
          if (material1.reflectivity > 0.0 && depth-1 > 0) {
              Ray reflectedRay1 = ray_reflect(&reflectedRay, closestHit1.surface, collisionPoint1);
              Color reflectionColor1;
              // Recurse level 2
              TracingResult closestHit2 = ray_traceOnce(&reflectedRay1, scene);
              Color resultColor2;
              if (closestHit2.surface == NULL) {
                  reflectionColor1 = scene->backgroundColor;
              }
              else
              {
                resultColor2 = closestHit2.surface->material.color;
                Vector3 collisionPoint2 = vec3_add(vec3_mult(reflectedRay1.direction, closestHit2.distance), reflectedRay1.origin);
                Material material2 = closestHit2.surface->material;
                if (material2.reflectivity > 0.0 && depth-2 > 0) {
                    Ray reflectedRay2 = ray_reflect(&reflectedRay1, closestHit2.surface, collisionPoint2);
                    Color reflectionColor2;
                    // Recurse level 3
                    TracingResult closestHit3 = ray_traceOnce(&reflectedRay2, scene);
                    Color resultColor3;
                    if (closestHit3.surface == NULL) {
                        reflectionColor2 = scene->backgroundColor;
                    }
                    else
                    {
                      resultColor3 = closestHit3.surface->material.color;
                      Vector3 collisionPoint3 = vec3_add(vec3_mult(reflectedRay2.direction, closestHit3.distance), reflectedRay2.origin);
                      Material material3 = closestHit3.surface->material;
                      if (material3.reflectivity > 0.0 && depth-3 > 0) {
                          Ray reflectedRay3 = ray_reflect(&reflectedRay2, closestHit3.surface, collisionPoint3);
                          Color reflectionColor3;
                          // Recurse level 4
                          TracingResult closestHit4 = ray_traceOnce(&reflectedRay3, scene);
                          Color resultColor4;
                          if (closestHit4.surface == NULL) {
                              reflectionColor3 = scene->backgroundColor;
                          }
                          else
                          {
                            resultColor4 = closestHit4.surface->material.color;
                            Vector3 collisionPoint4 = vec3_add(vec3_mult(reflectedRay3.direction, closestHit4.distance), reflectedRay3.origin);
                            Material material4 = closestHit4.surface->material;
                            
                            ShadingResult shadingResult4 = ray_shadeAtPoint(&reflectedRay3, scene, closestHit4.surface, collisionPoint4);    
                            resultColor4 = getHighlightedColor(resultColor4, shadingResult4, scene->ambientCoefficient);
                            resultColor4 = color_mult(resultColor4, (MAX_VISIBLE_DISTANCE - closestHit4.distance) / MAX_VISIBLE_DISTANCE);
                            reflectionColor3 = resultColor4;
                          }                          
                          // End of recurse level 4
                          resultColor3 = color_blend(reflectionColor3, material3.reflectivity, resultColor3);
                      }
                      ShadingResult shadingResult3 = ray_shadeAtPoint(&reflectedRay2, scene, closestHit3.surface, collisionPoint3);    
                      resultColor3 = getHighlightedColor(resultColor3, shadingResult3, scene->ambientCoefficient);
                      resultColor3 = color_mult(resultColor3, (MAX_VISIBLE_DISTANCE - closestHit3.distance) / MAX_VISIBLE_DISTANCE);
                      reflectionColor2 = resultColor3;
                    }
                    // End of recurse level 3
                    resultColor2 = color_blend(reflectionColor2, material1.reflectivity, resultColor2);
                }
                ShadingResult shadingResult2 = ray_shadeAtPoint(&reflectedRay1, scene, closestHit2.surface, collisionPoint2);    
                resultColor2 = getHighlightedColor(resultColor2, shadingResult2, scene->ambientCoefficient);
                resultColor2 = color_mult(resultColor2, (MAX_VISIBLE_DISTANCE - closestHit2.distance) / MAX_VISIBLE_DISTANCE);
                reflectionColor1 = resultColor2;
              }                                    
              // End of recurse level 2
              resultColor1 = color_blend(reflectionColor1, material1.reflectivity, resultColor1);
          }
          ShadingResult shadingResult1 = ray_shadeAtPoint(&reflectedRay, scene, closestHit1.surface, collisionPoint1);    
          resultColor1 = getHighlightedColor(resultColor1, shadingResult1, scene->ambientCoefficient);
          resultColor1 = color_mult(resultColor1, (MAX_VISIBLE_DISTANCE - closestHit1.distance) / MAX_VISIBLE_DISTANCE);
          reflectionColor = resultColor1;
        }
        // End of recurse level 1
        resultColor = color_blend(reflectionColor, material.reflectivity, resultColor);
    }
    ShadingResult shadingResult = ray_shadeAtPoint(ray, scene, closestHit.surface, collisionPoint);    
    resultColor = getHighlightedColor(resultColor, shadingResult, scene->ambientCoefficient);
    resultColor = color_mult(resultColor, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    return resultColor;
}

static TracingResult ray_traceOnce(const Ray *ray, const Scene *scene) {
    TracingResult closestHit = { .surface = NULL, .distance = 1.0 / 0.0 };
    double distance = 1.0 / 0.0f;
    int hit = 0;
    

    for (size_t i = 0; i < scene->surfaces.count; i++) {
        Surface *surface = ARRAY_GET(&scene->surfaces, i);
        hit = ray_checkIntersection(ray, surface, &distance);
        if (hit && distance < closestHit.distance && distance > EPSILON) {
            closestHit.distance = distance;
            closestHit.surface = surface;
        }
    }
    
    return closestHit;
}

static ShadingResult ray_shadeAtPoint(const Ray *ray, const Scene *scene, const Surface *surface, Vector3 point) {
    size_t i;
    Light *light;
    ShadingResult shadingResult;
    shadingResult.diffused = 0.0;
    shadingResult.specular = 0.0;
    double lightDistance;
    Vector3 lightDirection;
    Ray newRay;
    newRay.origin = point;
    TracingResult shadowTracingResult;
    
    
        
    for (i = 0; i < scene->lights.count; i++) {
        light = ARRAY_GET(&scene->lights, i);
        lightDirection = light_getDirection(light->position, point);
        newRay.direction = vec3_negate(lightDirection);
        lightDistance = vec3_length(vec3_sub(light->position, point));
        shadowTracingResult = ray_traceOnce(&newRay, scene);
        if (shadowTracingResult.surface == NULL || shadowTracingResult.distance > lightDistance) {
            Vector3 normal = surface_getNormalAtPoint(surface, point);
            
            shadingResult.diffused += light_getDiffusedHighlight(light, lightDirection, normal);
            shadingResult.specular += light_getSpecularHighlight(light, lightDirection,
                                                                 normal, ray->direction,
                                                                 surface->material.specularity);
        }
    }
    
    
    
    
    return shadingResult;
}

// Removed as rand is incompatible with OpenACC. To fix this, the cuda rand function 
//   would have to be used, which would make it incompatible without OpenACC.
static Ray ray_addNoise(const Ray *ray, double epsilon) {
    double r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    Ray newRay = *ray;
    newRay.direction.x += r;
    r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    newRay.direction.y += r;
    r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    newRay.direction.z += r;
    newRay.direction = vec3_unit(ray->direction);
    return newRay;
}

static Ray ray_reflect(const Ray *ray, const Surface *surface, Vector3 point) {
    Vector3 surfaceNormal = surface_getNormalAtPoint(surface, point);
    double c1 = - vec3_dot(surfaceNormal, ray->direction);
    Vector3 RI = vec3_add(ray->direction, vec3_mult(surfaceNormal, 2 * c1));
    return ray_make(point, RI);
}

static int ray_checkIntersection(const Ray *ray, const Surface *surface, double *distance) {
    switch (surface->type) {
        case GTSphere:
            return ray_checkSphereIntersection(ray, &surface->geometry.sphere, distance);
            break;
        case GTTriangle:
            return ray_checkTriangleIntersection(ray, &surface->geometry.triangle, distance);
            break;
        default:
            break;
    }
    return 0;
}

// http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html


// http://stackoverflow.com/questions/14074643/why-does-raytracer-render-spheres-as-ovals
static int ray_checkSphereIntersection(const Ray *ray, const Sphere *sphere, double *distance) {
    double a = vec3_dot(ray->direction, ray->direction);
    Vector3 ro_sc = vec3_sub(ray->origin, sphere->center);
    double b = 2.0 * vec3_dot(ray->direction, ro_sc);
    double c = vec3_dot(ro_sc, ro_sc) - SQUARE(sphere->radius);
    double disc = SQUARE(b) - 4 * a * c;
    if (disc < 0) {
        return 0;
    }    
    double distSqrt = sqrt(disc);
    double q = b < 0.0 ? (-b - distSqrt) / 2.0 : (-b + distSqrt) / 2.0;
    double t0 = q / a;
    double t1 = c / q;
    if (t0 > t1) {
        double swap = t0;
        t0 = t1;
        t1 = swap;
    }
    if (t1 < 0) {
        return 0;
    } else if(t0 < 0) {
        *distance = t1;
    } else {
        *distance = t0;
    }
    return 1;
}

// http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
static int ray_checkTriangleIntersection(const Ray *ray, const Triangle *t, double *distance) {
    Vector3 pvec, tvec, qvec;
    VEC3_CROSS(pvec, ray->direction, t->edges[0]);
    double det = VEC3_DOT(t->edges[1], pvec);
    
#define EPSILON 0.000001
    if (det < EPSILON) {
        return 0;
    }
#undef EPSILON
    VEC3_SUB(tvec, ray->origin, t->a);
    double u = VEC3_DOT(tvec, pvec);
    if (u < 0.0 || u > det) {
        return 0;
    }
    VEC3_CROSS(qvec, tvec, t->edges[1]);
    double v = VEC3_DOT(ray->direction, qvec);
    if (v < 0.0 || u + v > det) {
        return 0;
    }
    double d = VEC3_DOT(t->edges[0], qvec);
    double inv_det = 1.0 / det;
    d *= inv_det;
    *distance = d;
    return 1;
}

Color getHighlightedColor(Color color, ShadingResult highlight, double ambientCoef) {
    double diffusedCoef = 1.0 - ambientCoef;
    color = color_mult(color, ambientCoef + highlight.diffused * diffusedCoef);
    color = color_add(color, color_mult(COLOR_WHITE, highlight.specular));
    return color;
}
