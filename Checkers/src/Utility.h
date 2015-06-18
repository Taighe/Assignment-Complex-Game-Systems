#ifndef _UTILITY_H
#define _UTILITY_H

#include <vector>
#include "Vertex.h"

typedef unsigned int GLuint;

unsigned int loadShader(char* a_vertexFile, unsigned int a_type);

bool loadShaders(char* a_vertexFile, char* a_fragmentFile, GLuint* a_result);
bool loadShaders(char* a_vertexFile, char* a_fragmentFile, char* a_geometryFile, GLuint* a_result);

#endif