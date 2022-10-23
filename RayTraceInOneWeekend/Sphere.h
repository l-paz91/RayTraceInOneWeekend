// -----------------------------------------------------------------------------
#ifndef SPHERE_H_
#define SPHERE_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Hittable.h"
#include "vec3.h"

// -----------------------------------------------------------------------------

class Sphere : public Hittable
{
public:
	// ---- constructors
	Sphere() {}
	Sphere(point3 pCenter, double pRadius, shared_ptr<Material> pMat)
		: mCenter(pCenter), mRadius(pRadius), mMatPtr(pMat)
	{}

	// ---- overrides

	// ---- methods
	virtual bool hit(const Ray& pRay, double pMinT, double pMaxT, HitRecord& pRecord) const override;

	// ---- members
	point3 mCenter;
	double mRadius;
	shared_ptr<Material> mMatPtr;
};

// -----------------------------------------------------------------------------

bool Sphere::hit(const Ray& pRay, double pMinT, double pMaxT, HitRecord& pRecord) const
{
	vec3 oc = pRay.origin() - mCenter;
	auto a = pRay.direction().lengthSquared();
	auto halfB = dot(oc, pRay.direction());
	auto c = oc.lengthSquared() - mRadius * mRadius;

	auto discriminant = halfB * halfB - a * c;
	if (discriminant < 0)
		return false;
	auto sqrtd = sqrt(discriminant);

	// find the nearest root that lies in the acceptable range
	auto root = (-halfB - sqrtd) / a;
	if (root < pMinT || pMaxT < root)
	{
		root = (-halfB + sqrtd) / a;
		if (root < pMinT || pMaxT < root)
			return false;
	}

	pRecord.mTrace = root;
	pRecord.mPoint = pRay.at(pRecord.mTrace);
	vec3 outwardNormal = (pRecord.mPoint - mCenter) / mRadius;
	pRecord.setFaceNormal(pRay, outwardNormal);
	pRecord.mMatPtr = mMatPtr;

	return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !SPHERE_H_
