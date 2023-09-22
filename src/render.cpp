#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "resource.hpp"
#include "swapchain.hpp"

#include <array>
#include <iostream>

#include "render.hpp"

namespace kt {
namespace render {

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
               const bool &framebufferResized) {

  vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE,
                  UINT64_MAX);
  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
      VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain(device, physicalDevice, surface, swapChain,
                      swapChainExtent, swapChainImageFormat, swapChainImages,
                      swapChainImageViews, colorImageView, depthImageView,
                      colorImage, depthImage, colorImageMemory,
                      depthImageMemory, swapChainFrameBuffers, msaaSamples,
                      renderPass, window);
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image.");
  }

  // Reset only if work is submitted
  vkResetFences(device, 1, &inFlightFences[currentFrame]);

  kt::resource::updateUniformBuffer(uniformBuffersMapped, swapChainExtent,
                                    currentFrame);

  vkResetCommandBuffer(commandBuffers[currentFrame], 0);
  kt::resource::recordCommandBuffer(
      commandBuffers[currentFrame], imageIndex, renderPass, swapChainExtent,
      swapChainFrameBuffers, graphicsPipeline, vertexBuffer, indexBuffer,
      indices, pipelineLayout, descriptorSets, currentFrame);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(graphicsQueue, 1, &submitInfo,
                    inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  VkSwapchainKHR swapChains[] = {swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;

  presentInfo.pResults = nullptr; // Optional

  result = vkQueuePresentKHR(presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      framebufferResized) {
    recreateSwapChain(device, physicalDevice, surface, swapChain,
                      swapChainExtent, swapChainImageFormat, swapChainImages,
                      swapChainImageViews, colorImageView, depthImageView,
                      colorImage, depthImage, colorImageMemory,
                      depthImageMemory, swapChainFrameBuffers, msaaSamples,
                      renderPass, window);
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  currentFrame = (currentFrame + 1) % kt::swapchain::MAX_FRAMES_IN_FLIGHT;
}

void createRenderPass(VkDevice &device, VkPhysicalDevice &physicalDevice,
                      VkRenderPass &renderPass, VkFormat &swapChainImageFormat,
                      VkSampleCountFlagBits &msaaSamples) {
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = swapChainImageFormat;
  colorAttachment.samples = msaaSamples;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = kt::resource::findDepthFormat(physicalDevice);
  depthAttachment.samples = msaaSamples;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachmentResolve{};
  colorAttachmentResolve.format = swapChainImageFormat;
  colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentResolveRef{};
  colorAttachmentResolveRef.attachment = 2;
  colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;
  subpass.pResolveAttachments = &colorAttachmentResolveRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  std::array<VkAttachmentDescription, 3> attachments = {
      colorAttachment, depthAttachment, colorAttachmentResolve};
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  }
}

void recreateSwapChain(
    VkDevice &device, VkPhysicalDevice &physicalDevice, VkSurfaceKHR &surface,
    VkSwapchainKHR &swapChain, VkExtent2D &swapChainExtent,
    VkFormat &swapChainImageFormat, std::vector<VkImage> &swapChainImages,
    std::vector<VkImageView> swapChainImageViews, VkImageView &colorImageView,
    VkImageView &depthImageView, VkImage &colorImage, VkImage &depthImage,
    VkDeviceMemory &colorImageMemory, VkDeviceMemory &depthImageMemory,
    std::vector<VkFramebuffer> &swapChainFrameBuffers,
    VkSampleCountFlagBits &msaaSamples, VkRenderPass &renderPass,
    GLFWwindow *window) {
  // Handle minimization
  int width = 0, height = 0;
  glfwGetFramebufferSize(window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device);

  kt::swapchain::cleanupSwapChain(device, colorImageView, colorImage,
                                  colorImageMemory, depthImageView, depthImage,
                                  depthImageMemory, swapChainFrameBuffers,
                                  swapChainImageViews, swapChain);

  kt::swapchain::createSwapChain(device, physicalDevice, surface, window,
                                 swapChain, swapChainImageFormat,
                                 swapChainExtent, swapChainImages);

  kt::resource::createSwapChainImageViews(
      device, swapChainImageViews, swapChainImages, swapChainImageFormat);

  kt::resource::createColorResources(
      device, physicalDevice, swapChainExtent, swapChainImageFormat,
      msaaSamples, colorImage, colorImageMemory, colorImageView);

  kt::resource::createDepthResources(device, physicalDevice, depthImage,
                                     depthImageView, depthImageMemory,
                                     msaaSamples, swapChainExtent);

  kt::resource::createFramebuffers(device, swapChainFrameBuffers,
                                   swapChainImageViews, colorImageView,
                                   depthImageView, renderPass, swapChainExtent);
}

} // namespace render
} // namespace kt