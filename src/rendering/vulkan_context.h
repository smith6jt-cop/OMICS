#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <optional>

namespace ct {

// Forward declaration
class Window;

/// Configuration for Vulkan context initialization
struct VulkanContextConfig {
    std::string applicationName = "Cellular Threshold";
    uint32_t applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    bool enableValidation = true;
};

/// Queue family indices for different queue types
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

/// RAII wrapper for Vulkan instance, device, and related resources
/// Handles Vulkan initialization, device selection, and validation layers
class VulkanContext {
public:
    VulkanContext() = default;
    ~VulkanContext();

    // Non-copyable
    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

    /// Initialize Vulkan with the given configuration
    /// @param config Vulkan configuration settings
    /// @param window Reference to the window for surface creation
    /// @return true if initialization succeeded
    bool initialize(const VulkanContextConfig& config, Window& window);

    /// Shutdown and release all Vulkan resources
    void shutdown();

    /// Wait for the device to be idle (useful before cleanup)
    void waitIdle();

    // Accessors for Vulkan handles
    [[nodiscard]] VkInstance getInstance() const { return m_instance; }
    [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }
    [[nodiscard]] VkDevice getDevice() const { return m_device; }
    [[nodiscard]] VkSurfaceKHR getSurface() const { return m_surface; }
    [[nodiscard]] VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
    [[nodiscard]] VkQueue getPresentQueue() const { return m_presentQueue; }
    [[nodiscard]] const QueueFamilyIndices& getQueueFamilyIndices() const { return m_queueFamilyIndices; }

private:
    /// Create the Vulkan instance
    bool createInstance(const VulkanContextConfig& config, Window& window);

    /// Set up the debug messenger for validation layers
    bool setupDebugMessenger();

    /// Create the window surface
    bool createSurface(Window& window);

    /// Select a suitable physical device (GPU)
    bool pickPhysicalDevice();

    /// Create the logical device and queues
    bool createLogicalDevice();

    /// Find queue families that support required operations
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    /// Check if a physical device is suitable for our needs
    bool isDeviceSuitable(VkPhysicalDevice device);

    /// Check if validation layers are available
    bool checkValidationLayerSupport();

    /// Get required device extensions
    std::vector<const char*> getRequiredExtensions(Window& window);

    /// Debug callback for validation layer messages
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    );

    /// Create debug utils messenger (loaded dynamically)
    VkResult createDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    );

    /// Destroy debug utils messenger (loaded dynamically)
    void destroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator
    );

    /// Populate debug messenger create info
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    // Vulkan handles
    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    QueueFamilyIndices m_queueFamilyIndices;
    bool m_validationEnabled = false;

    // Validation layer names
    const std::vector<const char*> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Required device extensions
    const std::vector<const char*> m_deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};

} // namespace ct
