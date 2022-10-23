// -----------------------------------------------------------------------------
// https://lptcp.blogspot.com/
//	Ray Tracing in One Weekend
/*
	13 - Where Next?
	13.1 A Final Render
*/
// https://github.com/l-paz91/
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Camera.h"
#include "colour.h"
#include "HittableList.h"
#include "Material.h"
#include "MultiThreadFunctions.h"
#include "rtweekend.h"
#include "Sphere.h"

#include <conio.h>
#include <iostream>

using namespace std;

// -----------------------------------------------------------------------------

int main()
{
	// image
	const auto aspectRatio = 16.0 / 9.0;
	const int image_width = 200;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int samplesPerPixel = 50;
	const int maxDepth = 50;

	// world
	auto world = randomScene();

	// camera
	point3 lookFrom(13, 2, 3);
	point3 lookAt(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto distToFocus = 10.0;
	auto aperture = 0.1;

	Camera camera(lookFrom, lookAt, vup, 20, aspectRatio, aperture, distToFocus);

	// render
	// cout << "P3\n" << pImageWidth << ' ' << pImageHeight << "\n255\n";
	//orginalRender(image_height, image_width, samplesPerPixel, maxDepth, camera, world);

	multithreadRender(image_height, image_width, samplesPerPixel, maxDepth, camera, world);

	cerr << "\nDone. \n";

	//_getch();
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
