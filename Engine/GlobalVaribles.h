#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Camera.h"
#include "Shader.h"
#include "stb/stb_image.h"
#include "Transformation.h"
#include "Texture.h"
#include "Model.h"

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
	bool movingXwing = false;
		
	void processInput(GLFWwindow* window);
	void RenderModel(Shader& shader, const Model& model, unsigned int texture, const glm::vec3& position, const glm::vec3& scale);
	unsigned int loadTexture(char const* path);
	unsigned int loadCubemap(std::vector<std::string> faces);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void MoveXwing(glm::vec3& xwingPos, const glm::vec3& targetPos);
	void RenderAABB(const Shader& shader, const glm::vec3& position, const glm::vec3& min, const glm::vec3& max);
};

#endif // !_GLOBAL_VARIABLES_H_

