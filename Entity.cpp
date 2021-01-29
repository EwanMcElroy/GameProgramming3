#include "pch.h"
#include "Entity.h"

Entity::Entity()
{
	m_transform = new Transform();
}

void Entity::AddComponent(Component* c)
{
	c->m_entity = this;
	m_components.push_back(c);
}

void Entity::OnUpdate(float deltaTime)
{
	for (auto& c : m_components)
	{
		c->OnUpdate(deltaTime);
	}
}

void Entity::OnRender()
{
	for (auto& c : m_components)
	{
		c->OnRender();
	}
}

void Entity::rotateAround(Entity* _obj, int rot)
{
	//glm::translate(glm::mat4(), _obj->GetTransform()->GetPosition());
	//glm::rotate(this->GetTransform()->GetRotation() , glm::vec3(0, rot, 0));
	//glm::translate(glm::mat4(), -_obj->GetTransform()->GetPosition());

	//glTranslatef(_obj->GetTransform()->GetPosition().x, _obj->GetTransform()->GetPosition().y, _obj->GetTransform()->GetPosition().z);
	//glRotatef(rot, 0, 1, 0);
	//glTranslatef(-_obj->GetTransform()->GetPosition().x, -_obj->GetTransform()->GetPosition().y, -_obj->GetTransform()->GetPosition().z);
}