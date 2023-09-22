#pragma once

#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN

#include <optional>
#include <vector>

namespace kt::device {

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

void pickPhysicalDevice(VkInstance &instance, VkPhysicalDevice &physicalDevice,
                        VkSampleCountFlagBits &msaaSamples,
                        VkSurfaceKHR &surface);

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR &surface);

bool checkDeviceExtensionSupport(VkPhysicalDevice &device);

void createLogicalDevice(VkDevice &device, VkPhysicalDevice physicalDevice,
                         VkQueue &presentQueue, VkQueue &graphicsQueue,
                         VkSurfaceKHR &surface);

kt::device::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                                 VkSurfaceKHR &surface);

VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);

std::vector<const char *> getRequiredExtensions();

} // namespace kt::device