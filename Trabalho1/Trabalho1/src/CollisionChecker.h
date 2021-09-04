#ifndef __COLLISION_CHECKER_H__
#define __COLLISION_CHECKER_H__

#include <memory>
#include "AABB.h"

namespace CollisionChecker
{
	static bool CheckCollisionBoxes(const std::unique_ptr<AABB> & E1, const std::unique_ptr<AABB> & E2)
	{		
		return (abs(E1->center.x - E2->center.x) > (E1->midLen.x + E2->midLen.x) ||
			(abs(E1->center.y - E2->center.y) > (E1->midLen.y + E2->midLen.y))) ? false : true;
	}
};

#endif // !__COLLISION_CHECKER_H__
