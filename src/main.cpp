#include "kraterapp.hpp"

#include <iostream>

int main() {
  kt::KraterAppCreateInfo createInfo{};
  kt::KraterApp app;
  app.loadCreateInfo(&createInfo);

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}