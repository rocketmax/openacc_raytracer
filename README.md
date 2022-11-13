
## About
This is a class project for CIS 5640 at Oakland University. The goal was to take an available code base that had a high computational load and runtime that could be optimized by adding parallelization with OpenACC. 

## Dependencies 
* [Nvidia HPC SDK 227](https://developer.nvidia.com/nvidia-hpc-sdk-227-downloads)
* LibPNG
* Tested on CentOS 3.10.0-1160.2.2.el7.x86_64 (OU Yoko Server)

## Build
The original raytracer project from kgabis offered two different rendering mechanisms. For simplicity, we use the libpng option. 
To build without OpenACC, use `make`. To build the OpenACC implementation, use `make_acc`

## Usage
There are three different scenes available, spheres, snowman, and teapot. Teapot is the most computationally intensive as it uses a mesh texture instead of spheres. This makes it a good target for benchmarking the OpenACC implementation. To run, the binary without OpenACC is `raytracer` and the one with is `raytracer_acc`. For example, to run the two to time them against each other, the commands could be
`./raytracer teapot`
`./raytracer_acc teapot`

## Files
* array
	* Defines a dynamically allocated array struct implementation
* camera
	* Sets up a simulated camera to trace rays from
* color
	* Struct of a three channel color (RGB) with helper functions
* csfml_render
	* Unused in our project
* geometry
	* Structs for the spheres and triangles used for 3D objects in the scene
* light 
	* Defines light sources and helper functions for the scene
* main
	* Sets resolution and starts the raytracer
* material
	* Struct for material characteristics (color, reflectivity, noise, specularity)
* mesh
	* Helper functions to load triangles from a file into an array that defines a 3D object
* png_render
	* Draws pixels into an output PNG file
* ray
	* Defines rays with an origin and direction and provides functions to perform the tracing of the ray
* raytracer
	* Initializes rays, traces the rays, and draws the defined color for each pixel. 
* scene
	* Struct that contains surfaces, lights, the camera, etc.
* surface
	* Struct that includes both spheres and triangles and helps to find normal vector at a point on the surface
* vector
	* 3D vector implementation and helper functions

## Credit
Credit to the bulk of the goes goes to kgabis and can be found here: [https://github.com/kgabis/raytracer](https://github.com/kgabis/raytracer)

## License
[The MIT License (MIT)](http://opensource.org/licenses/mit-license.php)
