#include "pch.h"
#include "CameraComp.h"
#include "Camera.h"
#include "Input.h"

void CameraComp::OnUpdate(float deltaTime)
{
}

void CameraComp::OnRender()
{
}

void CameraComp::Start()
{
	m_camera = new Camera(m_entity->GetTransform());

	if (Application::GetInstance()->GetCamera() == nullptr)
	{
		Application::GetInstance()->SetCamera(m_camera);
	}
}

void CameraComp::setCamera()
{
	m_camera = new Camera(m_entity->GetTransform()); // set to main camera
	Application::GetInstance()->SetCamera(m_camera);
}

void CameraComp::relRot(Entity* _target)
{
	m_camera->relativeRotation(_target); // attempt at realtive rotation
}


