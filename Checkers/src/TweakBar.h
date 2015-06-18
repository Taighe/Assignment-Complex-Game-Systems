#ifndef _TWEAK_H
#define _TWEAK_H

#include "AntTweakBar.h"

struct GLFWwindow;

void onMouseButton(GLFWwindow* a_Window, int a_Button, int a_Pressed, int a_AltKeys);

void onMousePosition(GLFWwindow* a_Window, double a_X, double a_Y);

void onMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y);

void onKey(GLFWwindow* a_Window, int a_Key, int a_Scancode, int a_Pressed, int a_AltKeys);

void onChar(GLFWwindow* a_Window, unsigned int a_C);

void onWindowResize(GLFWwindow* a_Window, int a_Width, int a_Height);

#endif