// -----------------------------------------------------------------------------
#ifndef HITTABLE_H_
#define HITTABLE_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "ray.h"
#include "rtweekend.h"

class Material;

// -----------------------------------------------------------------------------

struct HitRecord
{
	// ---- constructors

	// ---- overrides

	// ---- methods
	inline void setFaceNormal(const Ray& pRay, const vec3& pOutwardNormal)
	{
		mFrontFace = dot(pRay.direction(), pOutwardNormal) < 0;
		mNormal = mFrontFace ? pOutwardNormal : -pOutwardNormal;
	}

	// ---- members
	point3 mPoint;
	vec3 mNormal;
	shared_ptr<Material> mMatPtr;
	double mTrace;
	bool mFrontFace;
};

// -----------------------------------------------------------------------------
	
class Hittable
{
public:
	// ---- constructors

	// ---- overrides

	// ---- methods
	virtual bool hit(const Ray& pRay, double pMinT, double pMaxT, HitRecord& pRecord) const = 0;

	// ---- members
};


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !HITTABLE_H_