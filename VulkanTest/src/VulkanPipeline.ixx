module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

export module VulkanPipeline;

import <vulkan/vulkan_core.h>;
import VulkanDevice;

//std
import <iostream>;
import <fstream>;
import <stdexcept>;
import <cassert>;

export class VulkanPipe;

export struct PipelineConfigInfo
{
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

export class VulkanPipe
{
	static std::vector<char> readFile(const std::string& filepath)
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

	void createGraphicsPipeline(
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo)
	{
		assert(
			configInfo.pipelineLayout != VK_NULL_HANDLE &&
			"Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
		assert(
			configInfo.renderPass != VK_NULL_HANDLE
			&& "Cannot create graphics pipeline:: no renderPass provided in configInfo");

		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		createShaderModule(vertCode, &vertShaderModule);
		createShaderModule(fragCode, &fragShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShaderModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShaderModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		

		VkPipelineViewportStateCreateInfo viewportInfo{} ;

		//blandar saman uppsetningu viewports og það sem á að skera af
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &configInfo.viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &configInfo.scissor;



		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
		pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = configInfo.pipelineLayout;
		pipelineInfo.renderPass = configInfo.renderPass;
		pipelineInfo.subpass = configInfo.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(
			vEngineDevice.device(),
			VK_NULL_HANDLE,
			1, &pipelineInfo,
			nullptr,
			&graphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics pipeline	");
		}
	}

	VEngineDevice& vEngineDevice;
	VkPipeline graphicsPipeline;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(vEngineDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module");
		}
	}

public:

	VulkanPipe(
		VEngineDevice& device,
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) : vEngineDevice{ device }
	{
		createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	~VulkanPipe()
	{
		vkDestroyShaderModule(vEngineDevice.device(), vertShaderModule, nullptr);
		vkDestroyShaderModule(vEngineDevice.device(), fragShaderModule, nullptr);
		vkDestroyPipeline(vEngineDevice.device(), graphicsPipeline, nullptr);
	}

	VulkanPipe(const VulkanPipe&) = delete;
	void operator=(const VulkanPipe&) = delete;

	static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo;

		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		//ákveður uppsetningu "viewports"(sviðs) innan gluggan
		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;
		configInfo.viewport.width = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f;
		configInfo.viewport.maxDepth = 1.0f;

		//sker af(cuts) allt það sem er ekki á sjáanlegt af sviðinu í glugganum
		configInfo.scissor.offset = { 0,0 };
		configInfo.scissor.extent = { width, height };

		//Rasterization phase
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO; // (brýtur upp gefna rúmmálið í litla búta þar sem þau fara yfir hvort annað?)
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; //segir öllum Z gildum að ef þau fara undir 0 þá gilda þau sem 0 og sama gildir ef yfir 1
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;//segir Vulkan hvernig við viljum útfæra þeim þríhyrningum sem fara á sviðið
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; //segir Vulkan í hvaða röð út frá hvar horft er á þríhyrninginn á að túlka púnktana þeirra
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;//breytir depthbias
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		//multisample phase
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f;
		configInfo.multisampleInfo.pSampleMask = nullptr;
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

		//colorBlending phase
		configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f;
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {};
		configInfo.depthStencilInfo.back = {};

		return configInfo;
	}
};