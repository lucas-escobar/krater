#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kt {
namespace initializers {
void createVkInstance(VkInstance *pInstance);
}
} // namespace kt