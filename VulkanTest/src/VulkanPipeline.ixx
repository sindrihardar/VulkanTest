export module VulkanPipeline;


import <vector>;
import <string>;
//std
import <fstream>;
import <stdexcept>;

import <iostream>;


export void MyFunc();

export class VulkanPipe
{
private:
	static std::vector<char> readFile(const std::string& filepath);

	void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

public:
	VulkanPipe(const std::string& vertFilepath, const std::string& fragFilepath);
};

std::vector<char> readFile(const std::string& filepath)
{
	std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file: " + filepath);
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath)
{
	auto vertCode = readFile(vertFilepath);
	auto fragCode = readFile(fragFilepath);

	std::cout << "Vertex Shader Code Size " << vertCode.size() << '\n';
	std::cout << "Vertex Shader Code Size " << vertCode.size() << '\n';
}

