// -----------------------------------------------------------------------------
#ifndef MATERIAL_H_
#define MATERIAL_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Hittable.h"
#include "rtweekend.h"

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

class Material
{
public:
	// ---- constructors

	// ---- overrides

	// ---- methods
	virtual bool scatter(const Ray& pRay, const HitRecord& pRecord, colour& pAttenuation,
		Ray& pScatteredRay) const = 0;

	// ---- members
};

// -----------------------------------------------------------------------------

class Lambertian : public Material
{
public:
	// ---- constructors
	Lambertian(const colour& pAlbedo)
		: mAlbedo(pAlbedo)
	{}

	// ---- overrides

	// ---- methods
	virtual bool scatter(const Ray& pRay, const HitRecord& pRecord, colour& pAttenuation,
		Ray& pScatteredRay) const override
	{
		auto scatterDirection = pRecord.mNormal + randomUnitVector();

		// catch degenerate scatter direction
		if (scatterDirection.nearZero())
			scatterDirection = pRecord.mNormal;

		pScatteredRay = Ray(pRecord.mPoint, scatterDirection);
		pAttenuation = mAlbedo;
		return true;
	}

	// ---- members
	colour mAlbedo;
};

// -----------------------------------------------------------------------------

class Metal : public Material
{
public:
	// ---- constructors
	Metal(const colour& pAlbedo, double pFuzz)
		: mAlbedo(pAlbedo), mFuzz((pFuzz < 1) ? pFuzz : 1)
	{}

	// ---- overrides

	// ---- methods
	virtual bool scatter(const Ray& pRay, const HitRecord& pRecord, colour& pAttenuation,
		Ray& pScatteredRay) const override
	{
		vec3 reflected = reflect(unitVector(pRay.direction()), pRecord.mNormal);
		pScatteredRay = Ray(pRecord.mPoint, reflected + mFuzz*randomInUnitSphere());
		pAttenuation = mAlbedo;
		return (dot(pScatteredRay.direction(), pRecord.mNormal) > 0);
	}

	// ---- members
	colour mAlbedo;
	double mFuzz;
};

// -----------------------------------------------------------------------------

class Dielectric : public Material
{
public:
	// ---- constructors
	Dielectric(double pIndexOfRefraction)
		: mIndexOfRefraction(pIndexOfRefraction)
	{}

	// ---- overrides

	// ---- methods
	virtual bool scatter(const Ray& pRay, const HitRecord& pRecord, colour& pAttenuation,
		Ray& pScatteredRay) const override
	{
		pAttenuation = colour(1.0, 1.0, 1.0);
		double refractionRatio = pRecord.mFrontFace ? (1.0 / mIndexOfRefraction) : mIndexOfRefraction;

		vec3 unitDirection = unitVector(pRay.direction());
		double cosTheta = fmin(dot(-unitDirection, pRecord.mNormal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		vec3 direction;

		if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
			direction = reflect(unitDirection, pRecord.mNormal);
		else
			direction = refract(unitDirection, pRecord.mNormal, refractionRatio);

		pScatteredRay = Ray(pRecord.mPoint, direction);
		return true;
	}

	static double reflectance(double pCosine, double pRefIdx)
	{
		// use Schlick's approcimation for reflectance
		auto r0 = (1 - pRefIdx) / (1 + pRefIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0)*pow((1 - pCosine), 5);
	}

	// ---- members
	double mIndexOfRefraction;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !MATERIAL_H_