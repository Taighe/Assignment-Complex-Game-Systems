#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "gl_core_4_4.h"

struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();
	virtual bool startup();
	virtual bool update();
	virtual void draw();
	virtual void shutdown();

protected:
	GLFWwindow* m_Window;
	int m_WindowWidth;
	int m_WindowHeight;
};

#endif