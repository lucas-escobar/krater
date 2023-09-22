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