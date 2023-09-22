#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "debug.hpp"
#include "device.hpp"

namespace kt::initializers {

void createVkInstance(VkInstance *pInstance) {
  if (kt::debug::enableValidationLayers &&
      !kt::debug::checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested but not available.");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Krater App";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  // use glfw extensions
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions = kt::device::getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (kt::debug::enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(kt::debug::validationLayers.size());
    createInfo.ppEnabledLayerNames = kt::debug::validationLayers.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    kt::debug::DebugMessenger::populateDebugMessengerCreateInfo(
        debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, pInstance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance.");
  }
}
} // namespace kt::initializers