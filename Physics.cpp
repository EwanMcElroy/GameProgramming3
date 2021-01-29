#include "pch.h"
#include "Physics.h"
#include "RigidBody.h"

Physics* Physics::m_instance = nullptr;

Physics::Physics()
{
	Init();
}

void Physics::Init()
{
	m_world->setGravity(btVector3(0, -9.8, 0));
}

void Physics::AddRigidbody(RigidBody* r)
{
	m_rbodies.push_back(r);
	m_world->addRigidBody(r->Get());
}

void Physics::PreUpdate()
{
	for (auto r : m_rbodies)
	{
		r->UpdateRigidBody();
	}
}

void Physics::Update(float deltaTime)
{
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);
	for (auto r : m_rbodies)
	{
		r->UpdateParent();
	}
}

void Physics::Quit()
{
	delete m_world;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfig;
}

btTransform Physics::ConvertTransformToBtTransform(Transform t)
{
	glm::quat rot = t.GetRotation();
	glm::vec3 pos = t.GetPosition();
	return btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z));
}

Physics* Physics::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Physics();
	}
	return m_instance;
}

bool Physics::collision(btCollisionObject* Obj1, btCollisionObject* obj2)
{
	if ((Obj1->getBroadphaseHandle()->m_aabbMax.x() < obj2->getBroadphaseHandle()->m_aabbMin.x()) || (Obj1->getBroadphaseHandle()->m_aabbMin.x() > obj2->getBroadphaseHandle()->m_aabbMax.x())) { return false; } // get the minimaum and maximum points of the collision shapes and return true if the points interlope
	if ((Obj1->getBroadphaseHandle()->m_aabbMax.y() < obj2->getBroadphaseHandle()->m_aabbMin.y()) || (Obj1->getBroadphaseHandle()->m_aabbMin.y() > obj2->getBroadphaseHandle()->m_aabbMax.y())) { return false; }
	if ((Obj1->getBroadphaseHandle()->m_aabbMax.z() < obj2->getBroadphaseHandle()->m_aabbMin.z()) || (Obj1->getBroadphaseHandle()->m_aabbMin.z() > obj2->getBroadphaseHandle()->m_aabbMax.z())) { return false; }
	return true;
}
