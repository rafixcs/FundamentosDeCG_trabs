#ifndef __COLLISION_CHECKER_H__
#define __COLLISION_CHECKER_H__

#include <memory>
#include <math.h>
#include "AABB.h"

namespace CollisionChecker
{
	static bool CheckCollisionBoxes(const AABB * E1, const AABB * E2)
	{
		return (fabs(E1->center.x - E2->center.x) > (E1->midLen.x + E2->midLen.x) ||
			(fabs(E1->center.y - E2->center.y) > (E1->midLen.y + E2->midLen.y))) ? false : true;
	}
};

#endif // !__COLLISION_CHECKER_H__
