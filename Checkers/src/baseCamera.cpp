#include "baseCamera.h"

Camera::Camera()
{	
	m_Position = vec3(0, 5, 10);
	m_ViewTransform = glm::lookAt(m_Position, vec3(0, 0, 0), vec3(0, 1, 0));
	m_ProjectionTransform = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);
	m_WorldTransform = glm::inverse(m_ViewTransform);
}

void Camera::update(float a_DeltaTime)
{			
	m_ViewTransform = glm::inverse(m_WorldTransform);
	m_ProjectionViewTransform = m_ProjectionTransform * m_ViewTransform;
}

void Camera::setPerspective(float a_FieldOfView, float a_Near, float a_Far)
{
	m_ProjectionTransform = glm::perspective(glm::radians(60.0f), a_FieldOfView, a_Near, a_Far);
}

void Camera::setLookAt(vec3 a_From, vec3 a_To, vec3 a_Up)
{
	m_ViewTransform = glm::lookAt(a_From, a_To, a_Up);
	m_WorldTransform = glm::inverse(m_ViewTransform);
}

mat4 Camera::setPosition(vec3 a_Position)
{
	m_Position = a_Position;
	m_WorldTransform = m_WorldTransform * glm::translate(m_Position);
	m_ViewTransform = glm::inverse(m_WorldTransform);
	return m_WorldTransform;
}

mat4 Camera::getWorldTransform()
{
	return m_WorldTransform;
}

mat4 Camera::getView()
{
	return m_ViewTransform;
}

mat4 Camera::getProjection()
{
	return m_ProjectionTransform;
}

mat4 Camera::getProjectionView()
{
	return m_ProjectionViewTransform;
}

void Camera::updateProjectionViewTransform()
{

}