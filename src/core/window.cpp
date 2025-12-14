#include "core/window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

namespace ct {

Window::~Window() {
    shutdown();
}

Window::Window(Window&& other) noexcept
    : m_window(other.m_window)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_framebufferResized(other.m_framebufferResized)
{
    other.m_window = nullptr;
    other.m_width = 0;
    other.m_height = 0;
    other.m_framebufferResized = false;

    // Update user pointer to new location
    if (m_window) {
        glfwSetWindowUserPointer(m_window, this);
    }
}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        shutdown();

        m_window = other.m_window;
        m_width = other.m_width;
        m_height = other.m_height;
        m_framebufferResized = other.m_framebufferResized;

        other.m_window = nullptr;
        other.m_width = 0;
        other.m_height = 0;
        other.m_framebufferResized = false;

        if (m_window) {
            glfwSetWindowUserPointer(m_window, this);
        }
    }
    return *this;
}

bool Window::initialize(const WindowConfig& config) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    // Check Vulkan support
    if (!glfwVulkanSupported()) {
        std::cerr << "Vulkan is not supported by GLFW\n";
        glfwTerminate();
        return false;
    }

    // Set window hints for Vulkan (no OpenGL context)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

    // Get primary monitor for fullscreen
    GLFWmonitor* monitor = nullptr;
    int windowWidth = static_cast<int>(config.width);
    int windowHeight = static_cast<int>(config.height);

    if (config.fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        windowWidth = mode->width;
        windowHeight = mode->height;
    }

    // Create the window
    m_window = glfwCreateWindow(
        windowWidth,
        windowHeight,
        config.title.c_str(),
        monitor,
        nullptr
    );

    if (!m_window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    // Store this pointer for callbacks
    glfwSetWindowUserPointer(m_window, this);

    // Set up framebuffer resize callback
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

    // Get actual framebuffer size (may differ from window size on high-DPI)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
    m_width = static_cast<uint32_t>(fbWidth);
    m_height = static_cast<uint32_t>(fbHeight);

    std::cout << "Window created: " << config.title
              << " (" << m_width << "x" << m_height << ")\n";

    return true;
}

void Window::shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
    }
    m_width = 0;
    m_height = 0;
    m_framebufferResized = false;
}

bool Window::shouldClose() const {
    return m_window && glfwWindowShouldClose(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

const char** Window::getRequiredInstanceExtensions(uint32_t* count) {
    return glfwGetRequiredInstanceExtensions(count);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_framebufferResized = true;
        app->m_width = static_cast<uint32_t>(width);
        app->m_height = static_cast<uint32_t>(height);
    }
}

} // namespace ct
