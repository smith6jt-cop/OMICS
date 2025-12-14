#include "core/engine.h"

#include <iostream>
#include <cstdlib>

int main() {
    ct::Engine engine;

    // Configure the engine
    ct::EngineConfig config;
    config.window.title = "Cellular Threshold";
    config.window.width = 1280;
    config.window.height = 720;
    config.window.resizable = true;
    config.applicationName = "Cellular Threshold";

#ifndef NDEBUG
    config.enableValidation = true;
#else
    config.enableValidation = false;
#endif

    // Initialize
    if (!engine.initialize(config)) {
        std::cerr << "Failed to initialize engine!\n";
        return EXIT_FAILURE;
    }

    std::cout << "Initialization successful. Running main loop...\n";

    // Run the main loop
    engine.run();

    // Cleanup handled by RAII
    return EXIT_SUCCESS;
}
