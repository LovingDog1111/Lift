#pragma once
#include "Vector3.h"

struct AABB {
	Vector3<float> lower;
	Vector3<float> upper;

	bool operator==(const AABB& other) const {
		return (lower.x == other.lower.x && lower.y == other.lower.y && lower.z == other.lower.z &&
			upper.x == other.upper.x && upper.y == other.upper.y && upper.z == other.upper.z);
	}

	Vector3<float> getCenter() const {
		return Vector3<float>(
			(lower.x + upper.x) * 0.5f,
			(lower.y + upper.y) * 0.5f,
			(lower.z + upper.z) * 0.5f
		);
	}

	bool intersects(const AABB& aabb) const {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

	bool intersectsY(const AABB& aabb) const {
		return aabb.upper.y > lower.y && upper.y > aabb.lower.y;
	}

	bool intersectsXZ(const AABB& aabb) const {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}
};
