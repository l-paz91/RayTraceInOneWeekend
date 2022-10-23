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
#include "rtweekend.h"
#include "Sphere.h"

#include <conio.h>
#include <iostream>

using namespace std;

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

int main()
{
	// image
	const auto aspectRatio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int samplesPerPixel = 100;
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
	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	const auto iw = image_width - 1;
	const auto ih = image_height - 1;

	for (int j = image_height - 1; j >= 0; --j)
	{
		cerr << "\rScanlines remaining: " << j << ' ' << flush;
		for (int i = 0; i < image_width; ++i)
		{
			colour pixelColour(0, 0, 0);
			for (int s = 0; s < samplesPerPixel; ++s)
			{
				auto u = (i + randomDouble()) / iw;
				auto v = (j + randomDouble()) / ih;
				Ray r = camera.getRay(u, v);
				pixelColour += rayColour(r, world, maxDepth);
			}
			writeColour(cout, pixelColour, samplesPerPixel);
		}
	}

	cerr << "\nDone. \n";

	//_getch();
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
