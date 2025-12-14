#include "core/engine.h"

#include <iostream>

namespace ct {

Engine::~Engine() {
    shutdown();
}

bool Engine::initialize(const EngineConfig& config) {
    std::cout << "Initializing Cellular Threshold Engine...\n";

    // Initialize window system
    if (!m_window.initialize(config.window)) {
        std::cerr << "Failed to initialize window system\n";
        return false;
    }

    // Configure Vulkan context
    VulkanContextConfig vulkanConfig;
    vulkanConfig.applicationName = config.applicationName;
    vulkanConfig.enableValidation = config.enableValidation;

    // Initialize Vulkan
    if (!m_vulkanContext.initialize(vulkanConfig, m_window)) {
        std::cerr << "Failed to initialize Vulkan context\n";
        m_window.shutdown();
        return false;
    }

    m_initialized = true;
    std::cout << "Engine initialization complete.\n";
    return true;
}

void Engine::run() {
    if (!m_initialized) {
        std::cerr << "Engine not initialized. Call initialize() first.\n";
        return;
    }

    std::cout << "Starting main loop...\n";
    m_running = true;

    while (m_running && !m_window.shouldClose()) {
        tick();
    }

    // Wait for GPU to finish before cleanup
    m_vulkanContext.waitIdle();

    std::cout << "Main loop ended.\n";
}

void Engine::shutdown() {
    if (!m_initialized) {
        return;
    }

    std::cout << "Shutting down engine...\n";
    m_running = false;

    // Shutdown in reverse order of initialization
    m_vulkanContext.shutdown();
    m_window.shutdown();

    m_initialized = false;
    std::cout << "Engine shutdown complete.\n";
}

void Engine::tick() {
    // Poll window events
    m_window.pollEvents();

    // Handle window resize
    if (m_window.wasResized()) {
        // TODO: Handle swapchain recreation
        m_window.resetResizeFlag();
    }

    // TODO: Update game logic

    // TODO: Render frame
}

} // namespace ct
