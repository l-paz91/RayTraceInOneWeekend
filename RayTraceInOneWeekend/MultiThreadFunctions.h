// -----------------------------------------------------------------------------
#ifndef MultiThreadFunctions_H
#define MultiThreadFunctions_H
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Camera.h"
#include "colour.h"
#include "HittableList.h"
#include "Material.h"
#include "rtweekend.h"
#include "Sphere.h"

#include <conio.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <sstream>

using namespace std;

#define MAX_NUM_THREADS 16

// -----------------------------------------------------------------------------

double hitSphere(const point3& pCenter, double pRadius, const Ray& pRay)
{
	vec3 oc = pRay.origin() - pCenter;
	auto a = pRay.direction().lengthSquared();
	auto halfB = dot(oc, pRay.direction());
	auto c = oc.lengthSquared() - pRadius * pRadius;
	auto discriminant = halfB * halfB - a * c;

	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-halfB - sqrt(discriminant)) / a;
	}
}

// -----------------------------------------------------------------------------

colour rayColour(const Ray& pRay, const Hittable& pWorld, int pDepth)
{
	// if we've exceeded the ray bounce limit, no more light it gathered
	if (pDepth <= 0)
		return colour(0, 0, 0);

	HitRecord rec;

	if (pWorld.hit(pRay, 0.001, gInfinity, rec))
	{
		Ray scattered;
		colour attentuation;

		if (rec.mMatPtr->scatter(pRay, rec, attentuation, scattered))
			return attentuation * rayColour(scattered, pWorld, pDepth - 1);
		return colour(0, 0, 0);
	}

	vec3 unitDirection = unitVector(pRay.direction());
	auto t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}

// -----------------------------------------------------------------------------

HittableList randomScene()
{
	HittableList world;

	auto matGround = make_shared<Lambertian>(colour(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, matGround));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			auto chooseMat = randomDouble();
			point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8)
				{
					// diffuse
					auto albedo = colour::random() * colour::random();
					sphereMaterial = make_shared<Lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95)
				{
					// metal
					auto albedo = colour::random(0.5, 1);
					auto fuzz = randomDouble(0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					// glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	auto material1 = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(colour(0.4, 0.2, 0.1));
	world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(colour(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

// -----------------------------------------------------------------------------

// this is the original render function from the book
// with a width of 200 and SPP set to 50, it takes around 3 minutes to render.
// it takes over an hour with a width of 400 and SPP set to 100
// this was run on an AMD Ryzen 9 3950x at the base speed of 3.5ghz (no overclocking)

// -----------------------------------------------------------------------------

void render(
	int pStartPosX, int pStartPosY,
	const int pChunkWidth, const int pChunkHeight,
	const int pImageHeight, 
	const int pImageWidth, 
	const int pSamplesPerPixel, 
	const int pMaxDepth,
	const int pThreadNumber,
	Camera pCamera, 
	HittableList pWorld,
	vector<vector<colour>>& pPixels)
{
	const int iw = pImageWidth - 1;
	const int ih = pImageHeight - 1;

	const int minY = pStartPosY - pChunkHeight;
	for (int j = pStartPosY - 1; j >= minY; --j)
	{
		const int maxX = pStartPosX + pChunkWidth;
		for (int i = pStartPosX; i < maxX; ++i)
		{
			colour pixelColour(0, 0, 0);
			for (int s = 0; s < pSamplesPerPixel; ++s)
			{
				auto u = (i + randomDouble()) / iw;
				auto v = (j + randomDouble()) / ih;
				Ray r = pCamera.getRay(u, v);
				pixelColour += rayColour(r, pWorld, pMaxDepth);
			}
			pPixels[j][i] = pixelColour;
		}
	}
}

// -----------------------------------------------------------------------------

// this is me attempting to multi thread the function
// it separates the pixels into a 4x4 grid. Each square writes to its own output file
// the output files are then concatenated after all the threads are done.
void multithreadRender(
	const int pImageHeight,
	const int pImageWidth,
	const int pSamplesPerPixel,
	const int pMaxDepth,
	Camera pCamera,
	HittableList pWorld)
{
	// returns a uint representing the number of concurrent threads that can be supported
	// by the hardware. It can return 0 if the number of hardware threads can't be
	// determined so we add a check for that
	const unsigned int numThreads = thread::hardware_concurrency() != 0
		? thread::hardware_concurrency() : 4;
	const int chunkHeight = pImageHeight / numThreads;

	vector<vector<colour>> pixels(pImageHeight, vector<colour>(pImageWidth));
	vector<thread> threads;

	for (uint32_t i = 0; i < numThreads; ++i)
	{
		int startY = pImageHeight - i * chunkHeight;
		threads.emplace_back(render, 0, startY, pImageWidth, chunkHeight, pImageHeight, pImageWidth,
			pSamplesPerPixel, pMaxDepth, i, pCamera, pWorld, ref(pixels));
	}

	for (thread& t : threads)
	{
		t.join();
	}

	// now stitch all the pixels together in one file
	ofstream file("output.ppm");
	file << "P3\n" << pImageWidth << ' ' << pImageHeight << "\n255\n";

	for (const auto& row : pixels)
	{
		for (const auto& pixel : row)
		{
			writeColour(file, pixel, pSamplesPerPixel);
		}
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !MultiThreadFunctions_H