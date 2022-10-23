// -----------------------------------------------------------------------------
#ifndef CAMERA_H_
#define CAMERA_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "rtweekend.h"

// -----------------------------------------------------------------------------

class Camera
{
public:
	// ---- constructors
	Camera(
		point3 pLookFrom, 
		point3 pLookAt, 
		vec3 pUp, 
		double pVerticalFOV, 
		double pAspectRatio,
		double pAperture,
		double pFocusDistance)
	{
		auto theta = deg2rad(pVerticalFOV);
		auto h = tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = pAspectRatio * viewportHeight;
		
		mW = unitVector(pLookFrom - pLookAt);
		mU = unitVector(cross(pUp, mW));
		mV = cross(mW, mU);

		mOrigin = pLookFrom;
		mHorizontal = pFocusDistance * viewportWidth * mU;
		mVertical = pFocusDistance * viewportHeight * mV;
		mLowerLeftCorner = mOrigin - mHorizontal / 2 - mVertical / 2 - pFocusDistance * mW;

		mLensRadius = pAperture / 2;
	}

	// ---- overrides

	// ---- methods
	Ray getRay(double pS, double pT) const
	{
		vec3 rd = mLensRadius * randomInUnitDisk();
		vec3 offset = mU * rd.x() + mV * rd.y();

		return Ray(mOrigin + offset, 
			mLowerLeftCorner + pS * mHorizontal + pT * mVertical - mOrigin - offset);
	}
	
private:
	// ---- members
	point3 mOrigin;
	point3 mLowerLeftCorner;
	vec3 mHorizontal;
	vec3 mVertical;
	vec3 mU, mV, mW;
	double mLensRadius;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !CAMERA_H_