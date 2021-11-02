
#include "../pch.h"

#include "Window.h"
#include "../Core/Time.h"

namespace Clumsy 
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
	static bool s_GLFWInitialized = false;

	Window::Window(int width, int height) :
		m_Width(width),
		m_Height(height)
	{
		m_IsCloseRequested = false;
		Init(width, height);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Init(int width, int height) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		GLFWwindow* window = glfwCreateWindow(width, height, "Disaster", NULL, NULL);
		//GLFWwindow* window = glfwCreateWindow(width, height, "Disaster", glfwGetPrimaryMonitor(), NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			//return -1;
		}

		m_GLFWWindow = window;
	}
	int Window::GetInput()
	{
		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			return 1;
		}
		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			return 2;
		}
		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_UP) == GLFW_PRESS)
		{
			return 3;
		}
		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			return 4;
		} 
	}
}