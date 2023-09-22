#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "debug.hpp"
#include "device.hpp"
#include "initializers.hpp"
#include "model.hpp"
#include "pipeline.hpp"
#include "render.hpp"
#include "resource.hpp"
#include "swapchain.hpp"
#include "window.hpp"

#include "kraterapp.hpp"

namespace kt {

KraterApp::KraterApp() {
  kt::KraterAppCreateInfo createInfo{};
  loadCreateInfo(&createInfo);
}

void KraterApp::loadCreateInfo(KraterAppCreateInfo *pCreateInfo) {
  kt::KraterApp::details.appName = pCreateInfo->appName;
  kt::KraterApp::details.appWidth = pCreateInfo->appWidth;
  kt::KraterApp::details.appHeight = pCreateInfo->appHeight;
}

void KraterApp::setFramebufferResized() {
  kt::KraterApp::context.framebufferResized = true;
}

void KraterApp::run() {
  initWindow(context);
  initVulkan(context);
  mainLoop(context);
  cleanup(context);
}

void KraterApp::initWindow(KtContext &c) {
  window::windowCreateInfo createInfo{};
  createInfo.windowName = details.appName;
  createInfo.windowWidth = details.appWidth;
  createInfo.windowHeight = details.appHeight;
  createInfo.windowUserPointer = this;
  createInfo.frameBufferResizeCallback = frameBufferResizeCallback;
  c.window = kt::window::createGlfwWindow(&createInfo);
}

void KraterApp::initVulkan(KtContext &c) {

  kt::initializers::createVkInstance(&c.instance);

  if (kt::debug::enableValidationLayers) {
    kt::debug::DebugMessenger::setup(c.instance, &c.debugMessenger);
  }

  kt::swapchain::createSurface(c.instance, c.window, c.surface);
  kt::device::pickPhysicalDevice(c.instance, c.physicalDevice, c.msaaSamples,
                                 c.surface);
  kt::device::createLogicalDevice(c.device, c.physicalDevice, c.presentQueue,
                                  c.graphicsQueue, c.surface);
  kt::swapchain::createSwapChain(c.device, c.physicalDevice, c.surface,
                                 c.window, c.swapchain, c.swapchainImageFormat,
                                 c.swapchainExtent, c.swapchainImages);
  kt::resource::createSwapChainImageViews(c.device, c.swapchainImageViews,
                                          c.swapchainImages,
                                          c.swapchainImageFormat);
  kt::render::createRenderPass(c.device, c.physicalDevice, c.renderPass,
                               c.swapchainImageFormat, c.msaaSamples);
  kt::resource::createDescriptorSetLayout(c.device, c.descriptorSetLayout);
  kt::pipeline::createGraphicsPipeline(c.device, c.graphicsPipeline,
                                       c.renderPass, c.graphicsPipelineLayout,
                                       c.descriptorSetLayout, c.msaaSamples);
  kt::resource::createCommandPool(c.commandPool, c.device, c.physicalDevice,
                                  c.surface);
  kt::resource::createColorResources(
      c.device, c.physicalDevice, c.swapchainExtent, c.swapchainImageFormat,
      c.msaaSamples, c.colorImage, c.colorImageMemory, c.colorImageView);
  kt::resource::createDepthResources(c.device, c.physicalDevice, c.depthImage,
                                     c.depthImageView, c.depthImageMemory,
                                     c.msaaSamples, c.swapchainExtent);
  kt::resource::createFramebuffers(
      c.device, c.swapchainFramebuffers, c.swapchainImageViews,
      c.colorImageView, c.depthImageView, c.renderPass, c.swapchainExtent);
  kt::resource::createCommandBuffer(c.commandBuffers, c.device, c.commandPool);
  kt::resource::createTextureImage(c.device, c.physicalDevice, c.commandPool,
                                   c.graphicsQueue, c.textureImage,
                                   c.textureImageMemory, c.mipLevels);
  kt::resource::createTextureImageView(c.device, c.textureImageView,
                                       c.textureImage, c.mipLevels);
  kt::resource::createTextureSampler(c.device, c.physicalDevice,
                                     c.textureSampler, c.mipLevels);
  kt::model::loadModel(c.vertices, c.indices);
  kt::resource::createVertexBuffer(c.device, c.physicalDevice, c.commandPool,
                                   c.graphicsQueue, c.vertices, c.vertexBuffer,
                                   c.vertexBufferMemory);
  kt::resource::createIndexBuffer(c.device, c.physicalDevice, c.commandPool,
                                  c.graphicsQueue, c.indices, c.indexBuffer,
                                  c.indexBufferMemory);
  kt::resource::createUniformBuffers(c.device, c.physicalDevice,
                                     c.uniformBuffers, c.uniformBuffersMemory,
                                     c.uniformBuffersMapped);
  kt::resource::createDescriptorPool(c.device, c.descriptorPool);
  kt::resource::createDescriptorSets(
      c.device, c.descriptorSetLayout, c.descriptorPool, c.descriptorSets,
      c.uniformBuffers, c.textureImageView, c.textureSampler);
  kt::resource::createSyncObjects(c.device, c.imageAvailableSemaphores,
                                  c.renderFinishedSemaphores, c.inFlightFences);
}

void KraterApp::mainLoop(KtContext &c) {
  while (!glfwWindowShouldClose(c.window)) {
    glfwPollEvents();
    kt::render::drawFrame(
        c.device, c.physicalDevice, c.surface, c.swapchain, c.swapchainExtent,
        c.swapchainImageFormat, c.graphicsPipeline, c.graphicsPipelineLayout,
        c.vertexBuffer, c.indexBuffer, c.indices, c.descriptorSets,
        c.swapchainImages, c.swapchainImageViews, c.swapchainFramebuffers,
        c.colorImage, c.colorImageView, c.colorImageMemory, c.depthImage,
        c.depthImageView, c.depthImageMemory, c.graphicsQueue, c.presentQueue,
        c.commandBuffers, c.inFlightFences, c.imageAvailableSemaphores,
        c.renderFinishedSemaphores, c.uniformBuffersMapped, c.renderPass,
        c.msaaSamples, c.window, c.currentFrame, c.framebufferResized);
  }

  // Wait for async operations to complete before exiting function
  vkDeviceWaitIdle(c.device);
}

void KraterApp::cleanup(KtContext &c) {
  kt::swapchain::cleanupSwapChain(
      c.device, c.colorImageView, c.colorImage, c.colorImageMemory,
      c.depthImageView, c.depthImage, c.depthImageMemory,
      c.swapchainFramebuffers, c.swapchainImageViews, c.swapchain);

  vkDestroyPipeline(c.device, c.graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(c.device, c.graphicsPipelineLayout, nullptr);
  vkDestroyRenderPass(c.device, c.renderPass, nullptr);

  for (size_t i = 0; i < kt::swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroyBuffer(c.device, c.uniformBuffers[i], nullptr);
    vkFreeMemory(c.device, c.uniformBuffersMemory[i], nullptr);
  }

  vkDestroyDescriptorPool(c.device, c.descriptorPool, nullptr);

  vkDestroySampler(c.device, c.textureSampler, nullptr);
  vkDestroyImageView(c.device, c.textureImageView, nullptr);

  vkDestroyImage(c.device, c.textureImage, nullptr);
  vkFreeMemory(c.device, c.textureImageMemory, nullptr);

  vkDestroyDescriptorSetLayout(c.device, c.descriptorSetLayout, nullptr);

  vkDestroyBuffer(c.device, c.indexBuffer, nullptr);
  vkFreeMemory(c.device, c.indexBufferMemory, nullptr);

  vkDestroyBuffer(c.device, c.vertexBuffer, nullptr);
  vkFreeMemory(c.device, c.vertexBufferMemory, nullptr);

  for (size_t i = 0; i < kt::swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(c.device, c.imageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(c.device, c.renderFinishedSemaphores[i], nullptr);
    vkDestroyFence(c.device, c.inFlightFences[i], nullptr);
  }

  vkDestroyCommandPool(c.device, c.commandPool, nullptr);
  vkDestroyDevice(c.device, nullptr);

  if (kt::debug::enableValidationLayers) {
    kt::debug::DebugMessenger::DestroyDebugUtilsMessengerEXT(
        c.instance, c.debugMessenger, nullptr);
  }

  vkDestroySurfaceKHR(c.instance, c.surface, nullptr);
  vkDestroyInstance(c.instance, nullptr);

  glfwDestroyWindow(c.window);

  glfwTerminate();
}

void KraterApp::frameBufferResizeCallback(GLFWwindow *window, int width,
                                          int height) {
  auto app = reinterpret_cast<KraterApp *>(glfwGetWindowUserPointer(window));
  app->setFramebufferResized();
}

} // namespace kt