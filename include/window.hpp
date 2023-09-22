#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kt {
namespace window {

struct windowCreateInfo {
  uint32_t windowWidth = 800;
  uint32_t windowHeight = 600;
  const char *windowName = "Kraken App";
  void *windowUserPointer = nullptr;
  GLFWframebuffersizefun frameBufferResizeCallback = NULL;
};

GLFWwindow *createGlfwWindow(windowCreateInfo *pCreateInfo) {
  glfwInit();

  // Disable OpenGL context creation
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWwindow *window =
      glfwCreateWindow(pCreateInfo->windowWidth, pCreateInfo->windowHeight,
                       pCreateInfo->windowName, nullptr, nullptr);

  glfwSetWindowUserPointer(window, pCreateInfo->windowUserPointer);
  glfwSetFramebufferSizeCallback(window,
                                 pCreateInfo->frameBufferResizeCallback);

  return window;
}

} // namespace window
} // namespace kt