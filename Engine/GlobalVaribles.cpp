#include "GlobalVaribles.h"

GlobalVariables GlobalVariables::_global;

void GlobalVariables::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void GlobalVariables::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GlobalVariables::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (GlobalVariables::_global.firstMouse)
    {
        GlobalVariables::_global.lastX = xpos;
        GlobalVariables::_global.lastY = ypos;
        GlobalVariables::_global.firstMouse = false;
    }

    float xoffset = xpos - GlobalVariables::_global.lastX;
    float yofffset = ypos - GlobalVariables::_global.lastY;

    GlobalVariables::_global.lastX = xpos;
    GlobalVariables::_global.lastY = ypos;

    GlobalVariables::_global.camera.ProcessMouseMovement(xoffset, yofffset);
}

void GlobalVariables::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GlobalVariables::_global.camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
