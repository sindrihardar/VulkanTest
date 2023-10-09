#include "VulkanTester.h"
#define GLFW_INCLUDE_VULKAN


VulkanTester::VulkanTester(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
{
	initWindow();
}

VulkanTester::~VulkanTester()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}


void VulkanTester::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

void Sanbox::run()
{
	while (!vulkanWindow.shouldClose())
	{
		glfwPollEvents();
	}
};

