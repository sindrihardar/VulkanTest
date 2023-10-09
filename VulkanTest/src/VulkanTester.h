#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

#include <iostream>

class VulkanTester
{
private:
	void initWindow();

	const int width;

	const int height;

    std::string windowName;
	GLFWwindow* window;

public:
	VulkanTester(int w, int h, std::string name);
	~VulkanTester();

	VulkanTester(const VulkanTester&) = delete;
	VulkanTester &operator=(const VulkanTester&) = delete;
  

	bool shouldClose() { return glfwWindowShouldClose(window); }
};


class Sanbox
{
private:
	VulkanTester vulkanWindow{WIDTH, HEIGHT, "Hello, Sindri!"};
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;


	void run();
};


