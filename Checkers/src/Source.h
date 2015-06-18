#ifndef _APP_H
#define _APP_H

#include "baseApp.h"
#include "Vertex.h"
#include "glm_header.h"
#include "TweakBar.h"
#include "Game.h"

const int MAX_MODELS = 2;

class Camera;

class App : public Application
{
public:
	App();
	virtual bool startup();
	virtual bool update();
	virtual void draw();
	virtual void shutdown();

	void generateGrid(float a_Size, int a_rows, int a_cols);
	void generatePlane(float a_Size);
	void generateFrameBuffer();
	unsigned int loadTexture(char* a_diffuse);
	void reloadShader();

private: 
	Camera* m_camera;
	TwBar* m_bar;

	Game* m_game;

	vec4 m_backgroundColor;
	vec3 m_ambientLight;
	vec3 m_lightDir;
	vec3 m_lightColor;
	vec3 m_materialColor; 
	float m_specularPower;
	
	int m_gizmoSize;
	float m_timer;

	//terrain vars
	float m_tHeight;
	int m_tOctaves;
	int m_tNoise;

	unsigned int m_program;
	unsigned int m_fbxShader;
	unsigned int m_perlinShader;
	unsigned int m_textureShader;

	unsigned int m_diffuseTexture;
	unsigned int m_normalTexture;

	unsigned int m_perlinTex;

	unsigned int m_fbo;
	unsigned int m_depth;
	
	bool m_keyDown;

	OpenGlData m_plane;

};

#endif