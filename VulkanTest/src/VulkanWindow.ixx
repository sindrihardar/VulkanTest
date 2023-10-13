module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
export module VulkanWindow;


import <stdexcept>;


export class VulkanWindow;

//-------- Create functions for window creation ------

export class VulkanWindow
{
private:
	const int width;

	const int height;

    std::string windowName;
	GLFWwindow* window;

	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}

public:

	VulkanWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
	{
		initWindow();
	}

	~VulkanWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	VulkanWindow(const VulkanWindow&) = delete;
	VulkanWindow &operator=(const VulkanWindow&) = delete;

	bool shouldClose() { return glfwWindowShouldClose(window); }

	VkExtent2D getExtent() { return{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
	{
		if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}
};


