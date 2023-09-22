#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <vector>

#include "resource.hpp"

namespace kt {

struct KraterAppCreateInfo {
  const char *appName = "Krater App";
  uint32_t appWidth = 800;
  uint32_t appHeight = 600;
};

class KraterApp {
public:
  KraterApp();
  void run();

  struct AppDetails {
    const char *appName;
    uint32_t appHeight;
    uint32_t appWidth;
  };
  AppDetails details;

  void loadCreateInfo(KraterAppCreateInfo *pCreateInfo);
  void setFramebufferResized();

private:
  struct KtContext {

    // top level
    GLFWwindow *window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;

    // debug
    VkDebugUtilsMessengerEXT debugMessenger;

    // physical device
    VkSurfaceKHR surface;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPhysicalDevice physicalDevice;

    // logical device
    VkQueue presentQueue;
    VkQueue graphicsQueue;
    VkDevice device;

    // swapchain
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    std::vector<VkImage> swapchainImages;
    VkSwapchainKHR swapchain;

    // resources
    std::vector<VkImageView> swapchainImageViews;

    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;

    VkPipeline graphicsPipeline;
    VkPipelineLayout graphicsPipelineLayout;

    VkCommandPool commandPool;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkFramebuffer> swapchainFramebuffers;

    std::vector<VkCommandBuffer> commandBuffers;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    uint32_t mipLevels = 1;
    VkSampler textureSampler;

    std::vector<kt::resource::Vertex> vertices;
    std::vector<uint32_t> indices;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    std::vector<VkBuffer> uniformBuffers;

    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;
    bool framebufferResized = false;
  };

  KtContext context;

  void initWindow(KtContext &c);
  void initVulkan(KtContext &c);
  void mainLoop(KtContext &c);
  void cleanup(KtContext &c);

  static void frameBufferResizeCallback(GLFWwindow *window, int width,
                                        int height);
};
} // namespace kt