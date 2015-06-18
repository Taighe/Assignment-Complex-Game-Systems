#ifndef _FLYCAM_H
#define _FLYCAM_H

#include "baseCamera.h"

class FlyCamera : public Camera
{
public:	
	FlyCamera();

	void update(float a_DeltaTime);

	void setSpeed(float a_Speed);

	vec3 pickAgainstPlane(float x, float y, const glm::vec4& plane);

private:
	bool m_EnableMouse;
	bool m_keyDown;
	vec3 m_Direction;
	float m_Speed;
	float m_Timer;
	double m_MousePosX;
	double m_MousePosY;
};

#endif