#pragma once
#include "Physics.h"

enum CShapeType
{
	BOX, SPHERE, CAPSULE, CYLINDER, CONE
};

class CollisionShape
{
protected:
	CShapeType type;
	btCollisionShape* m_shape;
	CollisionShape();
public:
	btCollisionShape* Get() { return m_shape; };
};

