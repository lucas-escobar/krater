# Krater (1.0.0-beta)

Krater is a Vulkan based rendering engine written in C++.

## Table of Contents
- [Getting Started](#getting-started)
    - [Install Dependencies](#install-dependencies)
    - [Compile & Build](#compile--build)
- [Usage](#usage)

## Getting Started

### Install Dependencies

1. Create  `./lib/`
2. Install the following:
    - Vulkan
    - GLFW
    - glm
    - stb
    - tinyobjectloader

3. ensure `CMakeLists.txt` correctly points to the install locations when setting `GLFW_LIBRARY`, `GLFW_INCLUDE_DIR`, and `glm_DIR`. 


### Compile & Build

1. Compile shaders `cd ./shaders && ./compile.bat`
2. Build Visual Studio files `cd ./build && cmake ..`
3. Open `./build/krater-app.sln` in Visual Studio for debug/release builds

## Usage
Krater exposes a single `kt::KraterApp` class.
```
#include "kraterapp.hpp"

#include <iostream>

int main() {
  kt::KraterApp app;
  kt::KraterAppCreateInfo createInfo{};
  createInfo.appName = "Krater App";
  createInfo.appHeight = 600;
  createInfo.appWidth = 800;
  app.loadCreateInfo(&createInfo);

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
```