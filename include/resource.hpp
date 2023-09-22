#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <array>

namespace kt::resource {

const std::string TEXTURE_PATH = "../textures/viking_room.png";
const uint32_t VERTEX_INPUT_BINDING_INDEX = 0;

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription();
  static ::std::array<VkVertexInputAttributeDescription, 3>
  getAttributeDescriptions();
  bool operator==(const Vertex &other) const;
};

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

void createCommandPool(VkCommandPool &commandPool, VkDevice &device,
                       VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface);

void createCommandBuffer(std::vector<VkCommandBuffer> &commandBuffers,
                         VkDevice &device, VkCommandPool &commandPool);

void recordCommandBuffer(VkCommandBuffer &commandBuffer, uint32_t &imageIndex,
                         VkRenderPass &renderPass, VkExtent2D &swapChainExtent,
                         std::vector<VkFramebuffer> &swapChainFramebuffers,
                         VkPipeline &graphicsPipeline, VkBuffer &vertexBuffer,
                         VkBuffer &indexBuffer, std::vector<uint32_t> &indices,
                         VkPipelineLayout &pipelineLayout,
                         std::vector<VkDescriptorSet> &descriptorSets,
                         uint32_t &currentFrame);

void createSyncObjects(VkDevice &device,
                       std::vector<VkSemaphore> &imageAvailableSemaphores,
                       std::vector<VkSemaphore> &renderFinishedSemaphores,
                       std::vector<VkFence> &inFlightFences);

void createBuffer(VkDevice &device, VkPhysicalDevice &physicalDevice,
                  VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory);

void createVertexBuffer(VkDevice &device, VkPhysicalDevice &physicalDevice,
                        VkCommandPool &commandPool, VkQueue &graphicsQueue,
                        std::vector<kt::resource::Vertex> &vertices,
                        VkBuffer &vertexBuffer,
                        VkDeviceMemory &vertexBufferMemory);

void createIndexBuffer(VkDevice &device, VkPhysicalDevice &physicalDevice,
                       VkCommandPool &commandPool, VkQueue &graphicsQueue,
                       std::vector<uint32_t> &indices, VkBuffer &indexBuffer,
                       VkDeviceMemory &indexBufferMemory);

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDevice &device,
                VkCommandPool &commandPool, VkDeviceSize size,
                VkQueue &graphicsQueue);

void copyBufferToImage(VkDevice &device, VkCommandPool &commandPool,
                       VkQueue &graphicsQueue, VkBuffer buffer, VkImage image,
                       uint32_t width, uint32_t height);

void createUniformBuffers(VkDevice &device, VkPhysicalDevice &physicalDevice,
                          std::vector<VkBuffer> &uniformBuffers,
                          std::vector<VkDeviceMemory> &uniformBuffersMemory,
                          std::vector<void *> &uniformBuffersMapped);

void createFramebuffers(VkDevice &device,
                        std::vector<VkFramebuffer> &swapChainFramebuffers,
                        std::vector<VkImageView> &swapChainImageViews,
                        VkImageView &colorImageView,
                        VkImageView &depthImageView, VkRenderPass &renderPass,
                        VkExtent2D &swapChainExtent);

void createDescriptorSetLayout(VkDevice &device,
                               VkDescriptorSetLayout &descriptorSetLayout);

void createDescriptorPool(VkDevice &device, VkDescriptorPool &descriptorPool);

void createDescriptorSets(VkDevice &device,
                          VkDescriptorSetLayout &descriptorSetLayout,
                          VkDescriptorPool &descriptorPool,
                          std::vector<VkDescriptorSet> &descriptorSets,
                          std::vector<VkBuffer> &uniformBuffers,
                          VkImageView &textureImageView,
                          VkSampler &textureSampler);

void createImage(VkDevice &device, VkPhysicalDevice &physicalDevice,
                 uint32_t width, uint32_t height, uint32_t mipLevels,
                 VkSampleCountFlagBits numSamples, VkFormat format,
                 VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties, VkImage &image,
                 VkDeviceMemory &imageMemory);

// This function does too much, needs refactor
void createTextureImage(VkDevice &device, VkPhysicalDevice &physicalDevice,
                        VkCommandPool &commandPool, VkQueue graphicsQueue,
                        VkImage &textureImage,
                        VkDeviceMemory &textureImageMemory,
                        uint32_t &mipLevels);

VkImageView createImageView(VkDevice &device, VkImage image, VkFormat format,
                            VkImageAspectFlags aspectFlags, uint32_t mipLevels);

void createSwapChainImageViews(VkDevice &device,
                               std::vector<VkImageView> &swapChainImageViews,
                               std::vector<VkImage> &swapChainImages,
                               VkFormat &swapChainImageFormat);

void createTextureImageView(VkDevice &device, VkImageView &textureImageView,
                            VkImage &textureImage, uint32_t &mipLevels);

void generateMipmaps(VkDevice &device, VkPhysicalDevice &physicalDevice,
                     VkCommandPool &commandPool, VkQueue &graphicsQueue,
                     VkImage image, VkFormat imageFormat, int32_t texWidth,
                     int32_t texHeight, uint32_t mipLevels);

VkCommandBuffer beginSingleTimeCommands(VkDevice &device,
                                        VkCommandPool &commandPool);

void createTextureSampler(VkDevice &device, VkPhysicalDevice &physicalDevice,
                          VkSampler &textureSampler, uint32_t mipLevels);

void createDepthResources(VkDevice &device, VkPhysicalDevice &physicalDevice,
                          VkImage &depthImage, VkImageView &depthImageView,
                          VkDeviceMemory &depthImageMemory,
                          VkSampleCountFlagBits &msaaSamples,
                          VkExtent2D &swapChainExtent);

void createColorResources(VkDevice &device, VkPhysicalDevice &physicalDevice,
                          VkExtent2D &swapChainExtent,
                          VkFormat &swapChainImageFormat,
                          VkSampleCountFlagBits &msaaSamples,
                          VkImage &colorImage, VkDeviceMemory &colorImageMemory,
                          VkImageView &colorImageView);

VkFormat findDepthFormat(VkPhysicalDevice &physicalDevice);

bool hasStencilComponent(VkFormat format);

VkFormat findSupportedFormat(VkPhysicalDevice &physicalDevice,
                             const std::vector<VkFormat> &candidates,
                             VkImageTiling tiling,
                             VkFormatFeatureFlags features);

void endSingleTimeCommands(VkDevice &device, VkCommandPool &commandPool,
                           VkCommandBuffer commandBuffer,
                           VkQueue &graphicsQueue);

void updateUniformBuffer(std::vector<void *> uniformBuffersMapped,
                         VkExtent2D &swapChainExtent, uint32_t currentImage);

uint32_t findMemoryType(VkPhysicalDevice &physicalDevice, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);

} // namespace kt::resource

namespace std {
template <> struct hash<kt::resource::Vertex> {
  size_t operator()(kt::resource::Vertex const &vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.texCoord) << 1);
  }
};
} // namespace std
