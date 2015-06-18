#ifndef _CAM_H
#define _CAM_H

#include <GLFW\glfw3.h>
#include "glm_header.h"

class Camera
{
public:
	Camera();
	virtual void update(float a_DeltaTime);
	virtual void setPerspective(float a_FieldOfView, float a_Near, float a_Far);
	virtual void setLookAt(vec3 a_From, vec3 a_To, vec3 a_Up);
	virtual mat4 setPosition(vec3 a_Position);
	virtual mat4 getWorldTransform();
	virtual mat4 getView();
	virtual mat4 getProjection();
	virtual mat4 getProjectionView();
	virtual void updateProjectionViewTransform();

protected:
	vec3 m_Position;
	mat4 m_WorldTransform;
	mat4 m_ViewTransform;
	mat4 m_ProjectionTransform;
	mat4 m_ProjectionViewTransform;
};

#endif