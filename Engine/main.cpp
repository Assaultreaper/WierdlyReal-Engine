#include "GlobalVaribles.h"
#include "Model.h"

const glm::vec2 WindowSize(GlobalVariables::_global.SCR_WIDTH, GlobalVariables::_global.SCR_HEIGHT);

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;

	}

	 stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

	Shader Shader("Assets/Shaders/Starwars.vs", "Assets/Shaders/Starwars.fs");

	Model xwing("xwing");
	unsigned int xwingTexture = GlobalVariables::_global.loadTexture("Assets/Models/xwing/xwing.jpg");
	
	Model StarShip("StarShip");
	unsigned int StartShipTexture = GlobalVariables::_global.loadTexture("Assets/Models/StarShip/StarShip.jpg");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		GlobalVariables::_global.deltaTime = currentFrame - GlobalVariables::_global.lastFrame;
		GlobalVariables::_global.lastFrame = currentFrame;

		GlobalVariables::_global.processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		Shader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(GlobalVariables::_global.camera.Zoom), (float)WindowSize.x/ (float)WindowSize.y, 0.1f, 10000.0f);
		glm::mat4 view = GlobalVariables::_global.camera.GetViewMatrix();
		Shader.setMat4("projection", projection);
		Shader.setMat4("view", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, StartShipTexture);
		Shader.setInt("texture0", 0);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1000.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		Shader.setMat4("model", model);
		StarShip.Draw(Shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, xwingTexture);
		Shader.setInt("texture0", 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, -10.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		Shader.setMat4("model", model);
		xwing.Draw(Shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
