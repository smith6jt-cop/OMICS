#pragma once

#include <string>
#include <cstdint>

// Forward declarations to avoid including GLFW in header
struct GLFWwindow;

namespace ct {

/// Configuration settings for window creation
struct WindowConfig {
    std::string title = "Cellular Threshold";
    uint32_t width = 1280;
    uint32_t height = 720;
    bool resizable = true;
    bool fullscreen = false;
    bool vsync = true;
};

/// RAII wrapper for GLFW window management
/// Handles window creation, destruction, and Vulkan surface creation
class Window {
public:
    Window() = default;
    ~Window();

    // Non-copyable
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Movable
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    /// Initialize the window with the given configuration
    /// @param config Window configuration settings
    /// @return true if initialization succeeded
    bool initialize(const WindowConfig& config = {});

    /// Shutdown and release all resources
    void shutdown();

    /// Check if the window should close (user clicked X, etc.)
    [[nodiscard]] bool shouldClose() const;

    /// Poll for window/input events
    void pollEvents();

    /// Get the raw GLFW window handle (for Vulkan surface creation)
    [[nodiscard]] GLFWwindow* getHandle() const { return m_window; }

    /// Get current window dimensions
    [[nodiscard]] uint32_t getWidth() const { return m_width; }
    [[nodiscard]] uint32_t getHeight() const { return m_height; }

    /// Check if window was resized since last frame
    [[nodiscard]] bool wasResized() const { return m_framebufferResized; }

    /// Reset the resize flag (call after handling resize)
    void resetResizeFlag() { m_framebufferResized = false; }

    /// Get required Vulkan instance extensions for GLFW surface creation
    /// @param count Output parameter for extension count
    /// @return Array of extension name strings
    static const char** getRequiredInstanceExtensions(uint32_t* count);

private:
    /// GLFW framebuffer resize callback
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window = nullptr;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    bool m_framebufferResized = false;
};

} // namespace ct
