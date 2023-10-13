export module VulkanApp;

import <GLFW/glfw3.h>;
import VulkanWindow;
import VulkanDevice;
import VulkanPipeline;

//------- Create Window -----------------
export class Sanbox
{
private:
	VulkanWindow vulkanWindow{ WIDTH, HEIGHT, "Hello, Sindri!" };
	VEngineDevice vEngineDevice{vulkanWindow};
	VulkanPipe vulkanPipe{vEngineDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", VulkanPipe::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;


	void run()
	{
			while (!vulkanWindow.shouldClose())
	{
		glfwPollEvents();
	}

	}
};
