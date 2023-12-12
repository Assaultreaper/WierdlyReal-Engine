#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Model.h"
#include "Physics.h"
#include "GlobalVaribles.h"

const glm::vec2 WindowSize(GlobalVariables::_global.SCR_WIDTH, GlobalVariables::_global.SCR_HEIGHT);

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlobalVariables::_global.OPEN_GL_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlobalVariables::_global.OPEN_GL_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow((int)WindowSize.x, (int)WindowSize.y, "Renderer", NULL, NULL);

    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GlobalVariables::_global.framebuffer_size_callback);
    glfwSetCursorPosCallback(window, GlobalVariables::_global.mouse_callback);
    glfwSetScrollCallback(window, GlobalVariables::_global.scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader modelShader("Assets/Shaders/UnlitShader_WithTextures.vs", "Assets/Shaders/UnlitShader_WithTextures.fs");

    Model xwing("xwing");
    unsigned int xwingTexture = GlobalVariables::_global.loadTexture("Assets/Models/xwing/xwing.jpg");

    Model starShip("StarShip");
    unsigned int starShipTexture = GlobalVariables::_global.loadTexture("Assets/Models/StarShip/StarShip.jpg");

    Physics::AABB xwingAABB;
    Physics::AABB starShipAABB;

    glm::vec3 xwingPos = glm::vec3(0.0f, -2.5f, -10.0f);
    glm::vec3 starShipPos = glm::vec3(0.0f, 100.0f, 1000.0f);  // Adjust the position as needed

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        GlobalVariables::_global.deltaTime = currentFrame - GlobalVariables::_global.lastFrame;
        GlobalVariables::_global.lastFrame = currentFrame;

        GlobalVariables::_global.processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 xwingScale = glm::vec3(1.0f, 1.0f, 1.0f);  // Adjust the scale as needed

        if (GlobalVariables::_global.movingXwing)
        {
            GlobalVariables::_global.MoveXwing(xwingPos, starShipPos);
        }

        glm::vec3 starShipScale = glm::vec3(1.0f, 1.0f, 1.0f);  // Adjust the scale as needed

        GlobalVariables::_global.RenderModel(modelShader, starShip, starShipTexture, starShipPos, starShipScale);
        GlobalVariables::_global.RenderModel(modelShader, xwing, xwingTexture, xwingPos, xwingScale);


        // Check collision
        if (Physics::CheckCollision(xwing.CalculateAABB(), starShip.CalculateAABB()))
        {
            std::cout << "Collision detected!" << std::endl;
            glfwSetWindowTitle(window, "Collision Detected");
        }
        else
        {
            std::cout << "No Collision" << std::endl;
            glfwSetWindowTitle(window, "No Collision");
        }

        // Render AABBs
        GlobalVariables::_global.RenderAABB(modelShader, starShipPos, starShipAABB.GetMinBounds(), starShipAABB.GetMaxBounds());
        GlobalVariables::_global.RenderAABB(modelShader, xwingPos, xwingAABB.GetMinBounds(), xwingAABB.GetMaxBounds());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
