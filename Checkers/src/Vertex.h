#ifndef VERTEX_H
#define VERTEX_H

#include "glm_header.h"

struct Vertex
{
	vec4 position;
	vec4 color;
};

struct TextureVertex
{
	vec4 position;
	vec2 texCoord;
};

struct nVertex
{
	vec4 position;	
	vec4 normal;
	vec4 tangent;
	vec2 texCoord;
};

struct OpenGlData
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_IndexCount;
};

#endif