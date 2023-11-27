#include "GlobalVaribles.h"
#include "Model.h"

const glm::vec2 WindowSize(GlobalVariables::_global.SCR_WIDTH, GlobalVariables::_global.SCR_HEIGHT);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlobalVariables::_global.OPEN_GL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlobalVariables::_global.OPEN_GL_VERSION);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow((int)WindowSize.x, (int)WindowSize.y, "Renderer", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, GlobalVariables::_global.framebuffer_size_callback);
	glfwSetCursorPosCallback(window, GlobalVariables::_global.mouse_callback);
	glfwSetScrollCallback(window, GlobalVariables::_global.scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Load GLAD so it configures OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader MaterialShader("VertexShader.glsl", "FragmentShader.glsl");

	Model Skull("Assets/Models/Skull/Skull.obj");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		GlobalVariables::_global.deltaTime = currentFrame - GlobalVariables::_global.lastFrame;
		GlobalVariables::_global.lastFrame = currentFrame;

		// input
		GlobalVariables::_global.processInput(window);
		
		// rendering commands here
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MaterialShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(GlobalVariables::_global.camera.Zoom), (float)WindowSize.x / (float)WindowSize.y, 0.1f, 100.0f);
		glm::mat4 view = GlobalVariables::_global.camera.GetViewMatrix();
		MaterialShader.setMat4("projection", projection);
		MaterialShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
		MaterialShader.setMat4("model", model);
		Skull.Draw(MaterialShader);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}