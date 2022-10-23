// -----------------------------------------------------------------------------
#ifndef RTWEEKEND_H_
#define RTWEEKEND_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// -----------------------------------------------------------------------------

// usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants
const double gInfinity = std::numeric_limits<double>::infinity();


// utility functions
const double getPI()
{
	double pi;
	__asm
	{
		fldpi
		fstp pi
	}
	return pi;
}

// -----------------------------------------------------------------------------

inline double deg2rad(double pDegrees)
{
	return pDegrees * getPI() / 180.0;
}

// -----------------------------------------------------------------------------

inline double randomDouble()
{
	// returns a random real in [0,1]
	return rand() / (RAND_MAX + 1.0);
}

// -----------------------------------------------------------------------------

inline double randomDouble(double pMin, double pMax)
{
	// returns a random real in [min, max]
	return pMin + (pMax - pMin) * randomDouble();
}

// -----------------------------------------------------------------------------

inline double clamp(double pX, double pMin, double pMax)
{
	if (pX < pMin)
		return pMin;
	if (pX > pMax)
		return pMax;

	return pX;
}

// common headers
#include "ray.h"
#include "vec3.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !RTWEEKEND_H_