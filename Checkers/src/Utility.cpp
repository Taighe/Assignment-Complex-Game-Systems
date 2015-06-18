#include "Utility.h"
#include "gl_core_4_4.h"
#include <cstdio>

unsigned int loadShader(char* a_vertexFile, unsigned int a_type)
{
	FILE* file = fopen(a_vertexFile, "rb");
	if (file == nullptr)
	{
		return 0;
	}
		
	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);
	
	unsigned int shader = glCreateShader(a_type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	
	delete[] source;
	return shader;
}

bool loadShaders(char* a_vertexFile, char* a_fragmentFile, GLuint* a_result)
{
	bool succeeded = false;

	FILE* vertexFile = fopen(a_vertexFile, "r");
	FILE* fragmentFile = fopen(a_fragmentFile, "r");

	if (vertexFile == 0 || fragmentFile == 0)
	{
		fclose(vertexFile);
		fclose(fragmentFile);
	}
	else
	{
		fseek(vertexFile, 0, SEEK_END);
		int vertexFileLength = ftell(vertexFile);
		fseek(vertexFile, 0, SEEK_SET);

		fseek(fragmentFile, 0, SEEK_END);
		int fragmentFileLength = ftell(fragmentFile);
		fseek(fragmentFile, 0, SEEK_SET);

		char* vsSource = new char[vertexFileLength];
		char* fsSource = new char[fragmentFileLength];

		vertexFileLength = fread(vsSource, 1, vertexFileLength, vertexFile);
		fragmentFileLength = fread(fsSource, 1, fragmentFileLength, fragmentFile);

		int success = GL_FALSE;
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertexShader, 1, &vsSource, &vertexFileLength);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShader, 1, &fsSource, &fragmentFileLength);
		glCompileShader(fragmentShader);

		*a_result = glCreateProgram();

		glAttachShader(*a_result, vertexShader);
		glAttachShader(*a_result, fragmentShader);
		//glBindAttribLocation(*a_result, 0, "position");
		//glBindAttribLocation(*a_result, 1, "TexCoord");
		glLinkProgram(*a_result);
		glGetProgramiv(*a_result, GL_LINK_STATUS, &success);

		succeeded = true;

		if (success == GL_FALSE)
		{
			int length = 0;
			char* infoLog = new char[length];
			glGetProgramiv(*a_result, GL_INFO_LOG_LENGTH, &length);
			glGetProgramInfoLog(*a_result, length, 0, infoLog);
			printf("ERROR: Failed to link shader program!\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
			succeeded = false;
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		fclose(vertexFile);
		fclose(fragmentFile);
	}


	return succeeded;
}

bool loadShaders(char* a_vertexFile, char* a_fragmentFile, char* a_geometryFile, GLuint* a_result)
{
	bool succeeded = false;

	FILE* vertexFile = fopen(a_vertexFile, "r");
	FILE* fragmentFile = fopen(a_fragmentFile, "r");
	FILE* geometryFile = fopen(a_geometryFile, "r");

	if (vertexFile == 0 || fragmentFile == 0 || geometryFile == 0)
	{
		fclose(vertexFile);
		fclose(fragmentFile);
	}
	else
	{
		fseek(vertexFile, 0, SEEK_END);
		int vertexFileLength = ftell(vertexFile);
		fseek(vertexFile, 0, SEEK_SET);

		fseek(fragmentFile, 0, SEEK_END);
		int fragmentFileLength = ftell(fragmentFile);
		fseek(fragmentFile, 0, SEEK_SET);

		fseek(geometryFile, 0, SEEK_END);
		int geometryFileLength = ftell(geometryFile);
		fseek(geometryFile, 0, SEEK_SET);

		char* vsSource = new char[vertexFileLength];
		char* fsSource = new char[fragmentFileLength];
		char* gsSource = new char[geometryFileLength];

		vertexFileLength = fread(vsSource, 1, vertexFileLength, vertexFile);
		fragmentFileLength = fread(fsSource, 1, fragmentFileLength, fragmentFile);
		geometryFileLength = fread(gsSource, 1, geometryFileLength, geometryFile);

		int success = GL_FALSE;
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		
		glShaderSource(vertexShader, 1, &vsSource, &vertexFileLength);
		glCompileShader(vertexShader);
		
		glShaderSource(fragmentShader, 1, &fsSource, &fragmentFileLength);
		glCompileShader(fragmentShader);

		glShaderSource(geometryShader, 1, &gsSource, &geometryFileLength);
		glCompileShader(geometryShader);

		*a_result = glCreateProgram();

		glAttachShader(*a_result, vertexShader);
		glAttachShader(*a_result, fragmentShader);
		glAttachShader(*a_result, geometryShader);
		//glBindAttribLocation(*a_result, 0, "position");
		//glBindAttribLocation(*a_result, 1, "TexCoord");
		glLinkProgram(*a_result);
		glGetProgramiv(*a_result, GL_LINK_STATUS, &success);

		succeeded = true;

		if (success == GL_FALSE)
		{
			int length = 0;
			glGetProgramiv(*a_result, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = new char[length];
			glGetProgramInfoLog(*a_result, length, 0, infoLog);
			printf("ERROR: Failed to link shader program!\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
			succeeded = false;
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		fclose(vertexFile);
		fclose(fragmentFile);
		fclose(geometryFile);
	}

	return succeeded;
}