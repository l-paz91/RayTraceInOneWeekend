// -----------------------------------------------------------------------------
#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Hittable.h"

#include <memory>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------

class HittableList : public Hittable
{
public:
	// ---- constructors
	HittableList() {}
	HittableList(shared_ptr<Hittable> pObject) { add(pObject); }

	// ---- overrides

	// ---- methods
	void clear() { mvObjects.clear(); }
	void add(shared_ptr<Hittable> pObject) { mvObjects.push_back(pObject); }

	virtual bool hit(const Ray& pRay, double pMinT, double pMaxT, HitRecord& pRecord) const override;

	// ---- members
	vector<shared_ptr<Hittable>> mvObjects;
};

// -----------------------------------------------------------------------------

bool HittableList::hit(const Ray& pRay, double pMinT, double pMaxT, HitRecord& pRecord) const
{
	HitRecord tempRec;
	bool hitAnything = false;
	auto closestSoFar = pMaxT;

	for (const auto& object : mvObjects)
	{
		if (object->hit(pRay, pMinT, closestSoFar, tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.mTrace;
			pRecord = tempRec;
		}
	}

	return hitAnything;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !HITTABLE_LIST_H_