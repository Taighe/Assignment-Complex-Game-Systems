#include "FlyCamera.h"
#include "TweakBar.h"

FlyCamera::FlyCamera()
{
	m_Speed = 20.0f;
	m_Direction = vec3();
	int width, height;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	glfwSetCursorPos(glfwGetCurrentContext(), width / 2, height / 2);
	m_Timer = 0;
	m_EnableMouse = false;
	m_keyDown = false;
}

vec3 FlyCamera::pickAgainstPlane(float x, float y, const glm::vec4& plane)
{
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_ProjectionTransform[0][0];
	screenPos.y /= m_ProjectionTransform[1][1];

	glm::vec3 dir = glm::normalize(m_WorldTransform * glm::vec4(screenPos, 0)).xyz();

	float d = (plane.w - glm::dot(m_WorldTransform[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	return m_WorldTransform[3].xyz() + dir * d;
}

void FlyCamera::update(float a_DeltaTime)
{
	m_Timer += a_DeltaTime;
	
	int width, height;
	
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	
	mat4 worldTransform = m_WorldTransform;

	vec3 velocity;
	
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS && m_EnableMouse == false)
	{
		glfwSetCursorPos(glfwGetCurrentContext(), width / 2, height / 2);
		m_EnableMouse = true;
	}	

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		m_EnableMouse = false;
	}

	glfwGetCursorPos(glfwGetCurrentContext(), &m_MousePosX, &m_MousePosY);

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		velocity.x = -m_Speed;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		velocity.x = m_Speed;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		velocity.z = -m_Speed;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		velocity.z = m_Speed;
	}
	
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R) == GLFW_PRESS)
	{
		velocity.y =  m_Speed;	
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F) == GLFW_PRESS)
	{
		velocity.y = -m_Speed;
	}
	
	worldTransform = setPosition(velocity * a_DeltaTime);

	if (m_MousePosY != height / 2 && m_EnableMouse)
	{
		float diff = (float)(m_MousePosY - height / 2);
		diff = diff * 0.1f;
		worldTransform = worldTransform * glm::rotate(-diff * a_DeltaTime, vec3(1, 0, 0));
	}
	if (m_MousePosX != width / 2 && m_EnableMouse)
	{
		float diff = (float)(m_MousePosX - width / 2);
		diff = diff * 0.1f;
		worldTransform = worldTransform * glm::rotate(-diff * a_DeltaTime, vec3(0, 1, 0));
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
	{
		worldTransform = worldTransform * glm::rotate(-m_Speed * a_DeltaTime * 0.1f, vec3(0, 0, 1));
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		worldTransform = worldTransform * glm::rotate(m_Speed * a_DeltaTime * 0.1f, vec3(0, 0, 1));
	}


	m_WorldTransform = worldTransform;

	if (m_EnableMouse)
		glfwSetCursorPos(glfwGetCurrentContext(), width / 2, height / 2);

	Camera::update(a_DeltaTime);
	
}