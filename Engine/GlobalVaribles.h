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
	static GlobalVariables _global;
	static const int SCR_WIDTH = 800;
	static const int SCR_HEIGHT = 800;

	const int OPEN_GL_VERSION = (int)4.5;

	Camera camera{(glm::vec3(0.0f, 0.0f, 3.0f))};
	float lastX = GlobalVariables::SCR_WIDTH / 2.0f;
	float lastY = GlobalVariables::SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif // !_GLOBAL_VARIABLES_H_

