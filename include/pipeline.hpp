#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace kt::pipeline {
static std::vector<char> readFile(const std::string &filename);

void createGraphicsPipeline(VkDevice &device, VkPipeline &graphicsPipeline,
                            VkRenderPass &renderPass,
                            VkPipelineLayout &pipelineLayout,
                            VkDescriptorSetLayout &descriptorSetLayout,
                            VkSampleCountFlagBits &msaaSamples);

VkShaderModule createShaderModule(VkDevice &device,
                                  const std::vector<char> &code);

} // namespace kt::pipeline