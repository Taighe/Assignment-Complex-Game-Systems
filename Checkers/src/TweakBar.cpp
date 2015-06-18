#include "TweakBar.h"
#include "gl_core_4_4.h"

void onMouseButton(GLFWwindow* a_Window, int a_Button, int a_Pressed, int a_AltKeys)
{
	TwEventMouseButtonGLFW(a_Button, a_Pressed);
}

void onMousePosition(GLFWwindow* a_Window, double a_X, double a_Y)
{
	TwEventMousePosGLFW((int)a_X, (int)a_Y);
}

void onMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y)
{
	TwEventMouseWheelGLFW((int)a_Y);
}

void onKey(GLFWwindow* a_Window, int a_Key, int a_Scancode, int a_Pressed, int a_AltKeys)
{
	TwEventKeyGLFW(a_Key, a_Pressed);
}

void onChar(GLFWwindow* a_Window, unsigned int a_C)
{
	TwEventCharGLFW(a_C, 1);
}

void onWindowResize(GLFWwindow* a_Window, int a_Width, int a_Height)
{
	TwWindowSize(a_Width, a_Height);
	glViewport(0, 0, a_Width, a_Height);
}