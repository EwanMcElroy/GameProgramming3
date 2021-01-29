#include "pch.h"
#include "RigidBody.h"
#include "Physics.h"
#include "Entity.h"

void RigidBody::OnUpdate(float deltaTime)
{
}

void RigidBody::OnRender()
{
}



void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3 localInertia)
{
	btTransform bT = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localInertia.x, localInertia.y, localInertia.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState, m_shape->Get(), li);
	Physics::GetInstance()->AddRigidbody(this);
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);
}

void RigidBody::UpdateParent()
{
	Transform* trans = m_entity->GetTransform();
	btTransform bT;
	m_rigidBody->getMotionState()->getWorldTransform(bT);
	btVector3 bPos = bT.getOrigin();
	glm::vec3 newPos = glm::vec3(bPos.getX(), bPos.getY(), bPos.getZ());
	trans->SetPosition(newPos);
	btQuaternion bRot = bT.getRotation();
	glm::quat newRot = glm::quat((float)bRot.getW(), (float)bRot.getX(), (float)bRot.getY(), (float)bRot.getZ());
	trans->SetRotation(newRot);
}

void RigidBody::UpdateRigidBody()
{
	btTransform t = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());

	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);
}

