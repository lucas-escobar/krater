#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kt::render {

void drawFrame(VkDevice &device, VkPhysicalDevice &physicalDevice,
               VkSurfaceKHR &surface, VkSwapchainKHR &swapChain,
               VkExtent2D &swapChainExtent, VkFormat &swapChainImageFormat,
               VkPipeline &graphicsPipeline, VkPipelineLayout &pipelineLayout,
               VkBuffer &vertexBuffer, VkBuffer &indexBuffer,
               std::vector<uint32_t> &indices,
               std::vector<VkDescriptorSet> &descriptorSets,
               std::vector<VkImage> &swapChainImages,
               std::vector<VkImageView> &swapChainImageViews,
               std::vector<VkFramebuffer> &swapChainFrameBuffers,
               VkImage &colorImage, VkImageView &colorImageView,
               VkDeviceMemory &colorImageMemory, VkImage &depthImage,
               VkImageView &depthImageView, VkDeviceMemory &depthImageMemory,
               VkQueue &graphicsQueue, VkQueue &presentQueue,
               std::vector<VkCommandBuffer> &commandBuffers,
               std::vector<VkFence> &inFlightFences,
               std::vector<VkSemaphore> &imageAvailableSemaphores,
               std::vector<VkSemaphore> &renderFinishedSemaphores,
               std::vector<void *> &uniformBuffersMapped,
               VkRenderPass &renderPass, VkSampleCountFlagBits &msaaSamples,
               GLFWwindow *window, uint32_t &currentFrame,
               const bool &framebufferResized);

void createRenderPass(VkDevice &device, VkPhysicalDevice &physicalDevice,
                      VkRenderPass &renderPass, VkFormat &swapChainImageFormat,
                      VkSampleCountFlagBits &msaaSamples);

void recreateSwapChain(
    VkDevice &device, VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface,
    VkSwapchainKHR &swapChain, VkExtent2D &swapChainExtent,
    VkFormat &swapChainImageFormat, std::vector<VkImage> &swapChainImages,
    std::vector<VkImageView> swapChainImageViews, VkImageView &colorImageView,
    VkImageView &depthImageView, VkImage &colorImage, VkImage &depthImage,
    VkDeviceMemory &colorImageMemory, VkDeviceMemory &depthImageMemory,
    std::vector<VkFramebuffer> &swapChainFrameBuffers,
    VkSampleCountFlagBits &msaaSamples, VkRenderPass &renderPass,
    GLFWwindow *window);

} // namespace kt::render