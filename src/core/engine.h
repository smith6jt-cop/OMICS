#pragma once

#include "core/window.h"
#include "rendering/vulkan_context.h"

#include <string>
#include <memory>

namespace ct {

/// Configuration for the game engine
struct EngineConfig {
    WindowConfig window;
    std::string applicationName = "Cellular Threshold";
    bool enableValidation = true;  // Enable Vulkan validation layers
};

/// Main game engine class
/// Manages the lifecycle of all engine subsystems
class Engine {
public:
    Engine() = default;
    ~Engine();

    // Non-copyable
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /// Initialize all engine systems
    /// @param config Engine configuration settings
    /// @return true if all systems initialized successfully
    bool initialize(const EngineConfig& config = {});

    /// Run the main engine loop
    void run();

    /// Shutdown all engine systems
    void shutdown();

    /// Check if the engine is currently running
    [[nodiscard]] bool isRunning() const { return m_running; }

    /// Get the window instance
    [[nodiscard]] Window& getWindow() { return m_window; }
    [[nodiscard]] const Window& getWindow() const { return m_window; }

    /// Get the Vulkan context instance
    [[nodiscard]] VulkanContext& getVulkanContext() { return m_vulkanContext; }
    [[nodiscard]] const VulkanContext& getVulkanContext() const { return m_vulkanContext; }

private:
    /// Process one frame
    void tick();

    Window m_window;
    VulkanContext m_vulkanContext;
    bool m_running = false;
    bool m_initialized = false;
};

} // namespace ct
