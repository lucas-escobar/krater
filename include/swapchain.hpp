#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kt::swapchain {

const int MAX_FRAMES_IN_FLIGHT = 2;

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

// namespace for return type is explicit because error otherwise
kt::swapchain::SwapChainSupportDetails
querySwapChainSupport(VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface);

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats);

VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D chooseSwapExtent(GLFWwindow *window,
                            const VkSurfaceCapabilitiesKHR &capabilities);

void createSwapChain(VkDevice &device, VkPhysicalDevice &physicalDevice,
                     VkSurfaceKHR &surface, GLFWwindow *window,
                     VkSwapchainKHR &swapChain, VkFormat &swapChainImageFormat,
                     VkExtent2D &swapChainExtent,
                     std::vector<VkImage> &swapChainImages);

void createSurface(VkInstance &instance, GLFWwindow *window,
                   VkSurfaceKHR &surface);

void cleanupSwapChain(VkDevice &device, VkImageView &colorImageView,
                      VkImage &colorImage, VkDeviceMemory &colorImageMemory,
                      VkImageView &depthImageView, VkImage &depthImage,
                      VkDeviceMemory &depthImageMemory,
                      std::vector<VkFramebuffer> &swapChainFramebuffers,
                      std::vector<VkImageView> &swapChainImageViews,
                      VkSwapchainKHR &swapChain);

} // namespace kt::swapchain