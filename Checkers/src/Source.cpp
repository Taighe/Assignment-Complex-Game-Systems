#include "Source.h"

#include "FlyCamera.h"
#include "Gizmos.h"
#include "Utility.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

App::App()
{
	
}

bool App::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glfwSetMouseButtonCallback(m_Window, onMouseButton);
	glfwSetCursorPosCallback(m_Window, onMousePosition);
	glfwSetScrollCallback(m_Window, onMouseScroll);
	glfwSetKeyCallback(m_Window, onKey);
	glfwSetCharCallback(m_Window, onChar);
	glfwSetWindowSizeCallback(m_Window, onWindowResize);

	Gizmos::create();

	glEnable(GL_DEPTH_TEST);
	
	m_camera = new FlyCamera();

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(m_WindowWidth, m_WindowHeight);
	m_bar = TwNewBar("GUI");

	TwAddVarRW(m_bar, "Clear Color", TW_TYPE_COLOR4F, &m_backgroundColor, "");
	TwAddVarRW(m_bar, "Direction", TW_TYPE_DIR3F, &m_lightDir, "group=Light");
	TwAddVarRW(m_bar, "Color", TW_TYPE_COLOR4F, &m_lightColor, "group=Light");
	TwAddVarRW(m_bar, "Specular Power", TW_TYPE_FLOAT, &m_specularPower, "group=Light min=0.1 max=100 step=0.5");

	m_backgroundColor = vec4(0.3, 0.3, 0.3, 1.0f);
	m_gizmoSize = 10;
	m_timer = 0;
	 
	m_ambientLight = vec3(0.1, 0.1, 0.1);
	m_lightDir = vec3(0, -1, 0);
	m_lightColor = vec3(1, 1, 1);
	m_materialColor = vec3(1, 1, 1);
	m_specularPower = 10.0f;

	m_diffuseTexture = loadTexture("./textures/board.png");
	m_normalTexture = loadTexture("./textures/board_n.png");
	
	loadShaders("./shaders/shader.vert", "./shaders/shader.frag", &m_program);

	//generateFrameBuffer();

	generatePlane(8);

	m_camera->setLookAt(vec3(0, 40, 0), vec3(0,0,0), vec3(0,0,-1));

	m_game = new Game(m_camera);

	return true;
}

bool App::update()
{
	if (Application::update() == false)
	{
		return false;
	}

	float dt = (float)glfwGetTime();
	m_timer += dt;
	glfwSetTime(0.0f);

	m_camera->update(dt);
	m_game->update();

	return true;
}

void App::draw()
{
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1);
	//glViewport(0, 0, 1280, 720);
	Gizmos::clear();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	glUseProgram(m_program);

	int loc = glGetUniformLocation(m_program, "projectionView");

	glUniformMatrix4fv(loc, 1, false, &m_camera->getProjectionView()[0][0]);

	loc = glGetUniformLocation(m_program, "lightDir");
	glUniform3fv(loc, 1, (float*)&m_lightDir);

	loc = glGetUniformLocation(m_program, "lightColor");
	glUniform3fv(loc, 1, (float*)&m_lightColor);

	loc = glGetUniformLocation(m_program, "ambientLight");
	glUniform3fv(loc, 1, (float*)&m_ambientLight);

	loc = glGetUniformLocation(m_program, "materialColor");
	glUniform3fv(loc, 1, (float*)&m_materialColor);

	loc = glGetUniformLocation(m_program, "eyePos");
	vec3 cameraPos = m_camera->getWorldTransform()[3].xyz;
	glUniform3fv(loc, 1, (float*)&cameraPos);

	loc = glGetUniformLocation(m_program, "specularPower");
	glUniform1f(loc, m_specularPower);

	//terrain
	loc = glGetUniformLocation(m_program, "terrainHeight");
	glUniform1f(loc, m_tHeight);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

	int diffLoc = glGetUniformLocation(m_program, "diffTexture");

	glUniform1i(diffLoc, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);

	int normLoc = glGetUniformLocation(m_program, "normTexture");

	glUniform1i(normLoc, 0);

	glBindVertexArray(m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, m_plane.m_IndexCount, GL_UNSIGNED_INT, 0);
	
	m_game->draw();
	
	TwDraw();
	
	Gizmos::draw(m_camera->getProjectionView());
	
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void App::shutdown()
{
	Gizmos::destroy();

	delete m_camera;
	m_camera = nullptr;

	delete m_game;
	m_game = nullptr;

	TwDeleteAllBars();
	TwTerminate();
	
	Application::shutdown();
}

void App::generateGrid(float a_Size, int a_rows, int a_cols)
{
	//setting up vertex data
	int vertices = (a_rows + 1) * (a_cols + 1);
	int quads = a_rows * a_cols;

	nVertex* vertex_data = nullptr;
	vertex_data = new nVertex[vertices];

	int i = 0;

	float offset = a_Size * 2;

	for (int y = 0; y < a_rows + 1; ++y)
	{
		for (int x = 0; x < a_cols + 1; ++x)
		{
			vertex_data[i].position = vec4(offset * x, 0, offset * y, 1);
			vertex_data[i].normal = vec4(0, 1, 0, 0);
			vertex_data[i].tangent = vec4(1, 0, 0, 0);
			vertex_data[i].texCoord = vec2((float)x / a_cols, (float)y / a_rows);

			i += 1;
		}
	}
	//
	//setting up index data

	i = 0;
	m_plane.m_IndexCount = quads * 6;
	unsigned int* index_data = nullptr;
	index_data = new unsigned int[m_plane.m_IndexCount];

	int currentIndex = 0;
	for (int y = 0; y < a_rows; ++y)
	{
		currentIndex = y * (1 + a_rows);
		for (int x = 0; x < a_cols; ++x)
		{
			//first triangle
			index_data[i] = currentIndex;
			index_data[i + 1] = currentIndex + (a_cols + 1);
			index_data[i + 2] = currentIndex + 1 + (a_cols + 1);

			//second triangle
			index_data[i + 3] = currentIndex;
			index_data[i + 4] = currentIndex + 1 + (a_cols + 1);
			index_data[i + 5] = currentIndex + 1;

			currentIndex += 1;
			i += 6;
		}

	}

	//Creating the buffers
	glGenVertexArrays(1, &m_plane.m_VAO); //vertex array object
	glGenBuffers(1, &m_plane.m_VBO); //vertex buffer object
	glGenBuffers(1, &m_plane.m_IBO); //index buffer object

	glBindVertexArray(m_plane.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_plane.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nVertex) * vertices, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_plane.m_IndexCount, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); // normal
	glEnableVertexAttribArray(2); // tangent
	glEnableVertexAttribArray(3); // tex coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(nVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(nVertex), (void*)(sizeof(vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(nVertex), (void*)(sizeof(vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(nVertex), (void*)(sizeof(vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[]vertex_data;
	delete[]index_data;
}

void App::generatePlane(float a_size)
{
	nVertex vertex_data[4];

	vertex_data[0].position = vec4(-a_size, 0, -a_size, 1);
	vertex_data[1].position = vec4(-a_size, 0, a_size, 1);
	vertex_data[2].position = vec4(a_size, 0, a_size, 1);
	vertex_data[3].position = vec4(a_size, 0, -a_size, 1);

	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[1].normal = vec4(0, 1, 0, 0);
	vertex_data[2].normal = vec4(0, 1, 0, 0);
	vertex_data[3].normal = vec4(0, 1, 0, 0);

	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[1].tangent = vec4(1, 0, 0, 0);
	vertex_data[2].tangent = vec4(1, 0, 0, 0);
	vertex_data[3].tangent = vec4(1, 0, 0, 0);

	vertex_data[0].texCoord = vec2(0, 0);
	vertex_data[1].texCoord = vec2(0, 1);
	vertex_data[2].texCoord = vec2(1, 1);
	vertex_data[3].texCoord = vec2(1, 0);

	unsigned int index_data[6] = { 0, 1, 2, 0, 2, 3 };
	m_plane.m_IndexCount = 6;

	//Creating the buffers
	glGenBuffers(1, &m_plane.m_VBO); //vertex buffer object
	glGenBuffers(1, &m_plane.m_IBO);//index buffer object
	glGenVertexArrays(1, &m_plane.m_VAO); //vertex array object

	glBindVertexArray(m_plane.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_plane.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nVertex)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); // normal
	glEnableVertexAttribArray(2); // tangent
	glEnableVertexAttribArray(3); // tex coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(nVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(nVertex), (void*)(sizeof(vec4)* 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(nVertex), (void*)(sizeof(vec4)* 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(nVertex), (void*)(sizeof(vec4)* 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void App::generateFrameBuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);

	// texture uses a 16-bit depth component format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// attached as a depth attachment to capture depth not colour
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth, 0);

	// no colour targets are used
	glDrawBuffer(GL_NONE);

	//Check for errors
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int App::loadTexture(char* a_diffuse)
{
	unsigned int texture = 0;
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(a_diffuse, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return texture;
}

void App::reloadShader()
{
	glDeleteProgram(m_program);
	loadShaders("./shaders/shader.vert", "./shaders/shader.frag", &m_program);
}