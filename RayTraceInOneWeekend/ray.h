// -----------------------------------------------------------------------------
#ifndef RAY_H_
#define RAY_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "vec3.h"

// -----------------------------------------------------------------------------

class Ray
{
public:
	// ---- constructors
	Ray() {}
	Ray(const point3& origin, const vec3& direction)
		: mOrig(origin)
		, mDir(direction)
	{}

	// ---- methods
	point3 origin() const { return mOrig; }
	vec3 direction() const { return mDir; }

	point3 at(double t) const
	{
		return mOrig + (t * mDir);
	}

	// ---- overrides

	// ---- members
	point3 mOrig;
	vec3 mDir;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !RAY_H_