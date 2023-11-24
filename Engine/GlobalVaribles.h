#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIBALES_H_

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb/stb_image.h"

#include "Shader.h"
#include "Transformation.h"
#include "Camera.h"

class GlobalVariables
{
public:
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 800;

	const int OPEN_GL_VERSION = (int)4.5;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void processInput(GLFWwindow* window);
};

#endif // !_GLOBAL_VARIABLES_H_

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

