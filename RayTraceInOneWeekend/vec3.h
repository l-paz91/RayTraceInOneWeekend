// -----------------------------------------------------------------------------
#ifndef VEC3_H_
#define VEC3_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <cmath>
#include <iostream>

// -----------------------------------------------------------------------------

using namespace std;

class vec3
{
public:
	// ---- constructors
	vec3() : e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// ---- methods
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	double length() const { return sqrt(lengthSquared()); }
	double lengthSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

	inline static vec3 random()
	{
		return vec3(randomDouble(), randomDouble(), randomDouble());
	}

	inline static vec3 random(double pMin, double pMax)
	{
		return vec3(randomDouble(pMin, pMax), randomDouble(pMin, pMax), randomDouble(pMin, pMax));
	}

	bool nearZero() const
	{
		// return true if the vector is close to zero in all dimensions
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	// ---- overrides
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3 &v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) { return *this *= 1 / t; }

	// ---- members
	double e[3];
};

// ---- type aliases for vec3
using point3 = vec3;	// 3D point
using colour = vec3;	// RGB colour

// -----------------------------------------------------------------------------

inline ostream& operator<<(ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// -----------------------------------------------------------------------------

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// -----------------------------------------------------------------------------


inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// -----------------------------------------------------------------------------

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// -----------------------------------------------------------------------------

inline vec3 operator*(double t, const vec3& v)
{
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// -----------------------------------------------------------------------------

inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

// -----------------------------------------------------------------------------

inline vec3 operator/(vec3 v, double t)
{
	return (1 / t) * v;
}

// -----------------------------------------------------------------------------

inline double dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0]
		 + u.e[1] * v.e[1]
		 + u.e[2] * v.e[2];
}

// -----------------------------------------------------------------------------

inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// -----------------------------------------------------------------------------

inline vec3 unitVector(vec3 v)
{
	return v / v.length();
}

// -----------------------------------------------------------------------------

vec3 randomInUnitSphere()
{
	while (true)
	{
		auto p = vec3::random(-1, 1);
		if (p.lengthSquared() >= 1)
			continue;
		return p;
	}
}

// -----------------------------------------------------------------------------

vec3 randomUnitVector()
{
	return unitVector(randomInUnitSphere());
}

// -----------------------------------------------------------------------------

vec3 randomInHemisphere(const vec3& pNormal)
{
	vec3 inUnitSphere = randomInUnitSphere();

	// in the same hemisphere as the normal
	if (dot(inUnitSphere, pNormal) > 0.0)
		return inUnitSphere;
	else
		return -inUnitSphere;
}

// -----------------------------------------------------------------------------

vec3 reflect(const vec3& pV, const vec3& pN)
{
	return pV - 2 * dot(pV, pN)*pN;
}

// -----------------------------------------------------------------------------

vec3 refract(const vec3& pUV, const vec3& pN, double pEtaiOverEtat)
{
	auto cosTheta = fmin(dot(-pUV, pN), 1.0);
	vec3 rOutPerp = pEtaiOverEtat * (pUV + cosTheta * pN);
	vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * pN;
	return rOutPerp + rOutParallel;
}

// -----------------------------------------------------------------------------

vec3 randomInUnitDisk()
{
	while (true)
	{
		auto p = vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
		if (p.lengthSquared() >= 1) continue;
		return p;
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !VEC3_H_