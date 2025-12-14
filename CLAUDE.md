# CLAUDE.md - Cellular Threshold Engine Development Guide

## Project Overview

**Cellular Threshold** is an educational game engine combining AAA-quality technical architecture with cellular biology themes. Players engage with immunology concepts through gameplay mechanics using immune cells as characters and biological tissues as environments.

**Repository**: smith6jt-cop/OMICS  
**Language**: C++20  
**Graphics API**: Vulkan 1.3+  
**Build System**: CMake 3.20+

## Quick Reference

### Build Commands

```bash
# Configure (from project root)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --config Debug

# Run
./build/bin/CellularThreshold

# Clean rebuild
rm -rf build && cmake -B build && cmake --build build
```

### Shader Compilation

```bash
# Manual shader compilation (automated in CMake)
glslc shaders/basic.vert -o build/bin/shaders/basic.vert.spv
glslc shaders/basic.frag -o build/bin/shaders/basic.frag.spv
```

### Validation Layer Debugging

```bash
# Enable verbose validation
export VK_LAYER_ENABLES=VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT

# Run with validation output
./build/bin/CellularThreshold 2>&1 | grep -E "\[Vulkan|Error|Warning"
```

## Code Architecture

### Namespace Convention

All engine code uses the `ct` namespace (Cellular Threshold):

```cpp
namespace ct {
    // All engine classes and functions
}
```

### File Organization

| Directory | Purpose |
|-----------|---------|
| `src/core/` | Window, input, engine loop |
| `src/rendering/` | Vulkan context, swapchain, pipeline |
| `src/rendering/multiplex_image/` | Multi-channel biological image processing |
| `src/ecs/` | Entity Component System |
| `src/asset_pipeline/` | Asset import and processing |
| `shaders/` | GLSL shader source files |
| `third_party/` | Git submodules (GLFW, GLM) |
| `assets/` | Runtime assets (models, textures) |

### Class Naming Patterns

- **Window management**: `Window`, `WindowConfig`
- **Vulkan wrappers**: `VulkanContext`, `Swapchain`, `Pipeline`
- **ECS**: `Component`, `System`, `EntityManager`
- **Asset handling**: `*Importer`, `*Processor`, `*Manager`

## Development Workflow

### Current Phase

Check `docs/DEVELOPMENT_PHASES.md` for current milestone target.

### Adding New Source Files

1. Create `.cpp` and `.h` in appropriate `src/` subdirectory
2. Add to `CMakeLists.txt` in the `engine_core` library target
3. Follow existing include patterns
4. Run build to verify compilation

### Adding Shaders

1. Create `.vert` or `.frag` in `shaders/`
2. Add to `compile_shaders()` call in root `CMakeLists.txt`
3. Load compiled `.spv` at runtime from `shaders/` relative to executable

## Code Standards

### RAII Pattern

All Vulkan resources must use RAII. Initialization in `initialize()`, cleanup in destructor and `shutdown()`:

```cpp
class VulkanResource {
public:
    bool initialize();
    void shutdown();
    ~VulkanResource() { shutdown(); }
private:
    VkHandle m_handle = VK_NULL_HANDLE;
};
```

### Error Handling

- Vulkan calls: Check `VkResult`, log on failure, return `false`
- Validation layers: Enabled in Debug builds via `#ifndef NDEBUG`
- Use `std::cerr` for error output (temporary until logging system)

### Memory Management

- Prefer RAII and smart pointers
- Vulkan allocations: Track all `vkCreate*` / `vkDestroy*` pairs
- No raw `new`/`delete` outside of explicit memory systems

## Testing Milestones

| Milestone | Verification |
|-----------|--------------|
| 1. Window + Vulkan | Window opens, GPU name printed |
| 2. Triangle | Colored triangle renders |
| 3. Textured Quad | Texture loads and displays |
| 4. Mesh Import | FBX/glTF mesh renders |
| 5. Multiplex Images | Multi-channel textures composite |

## Dependencies

### Required (System)

- Vulkan SDK 1.3+ with validation layers
- C++20 compiler (MSVC 2022, GCC 11+, Clang 14+)
- CMake 3.20+

### Submodules (third_party/)

```bash
git submodule update --init --recursive
```

- GLFW 3.3+ (window/input)
- GLM (mathematics)

### Future Dependencies

- Assimp (asset import)
- stb_image (texture loading)
- Dear ImGui (debug UI)

## Environment Variables

```bash
VULKAN_SDK=/path/to/vulkan/sdk
VK_LAYER_PATH=$VULKAN_SDK/etc/vulkan/explicit_layer.d
```

## Common Issues

### "Validation layers not available"

Ensure `VK_LAYER_PATH` points to validation layer manifests:
```bash
ls $VK_LAYER_PATH/*.json
```

### "Failed to create window surface"

GLFW requires Vulkan loader. Verify GLFW was built with Vulkan support:
```bash
cmake -DGLFW_VULKAN_STATIC=OFF ..
```

### Shader compilation fails

Verify `glslc` is in PATH:
```bash
which glslc
$VULKAN_SDK/bin/glslc --version
```

## Project Context

This engine supports the "Cellular Threshold" educational game featuring:

- **Player Character**: Kael-8, elite Cytotoxic T Cell (CD8+)
- **Environments**: Pancreatic islets, lymph nodes, spleen, thymus
- **Theme**: Immune system dysregulation and autoimmune cascade
- **Educational Goal**: Interactive immunology learning through gameplay

See `docs/NARRATIVE.md` for full story framework.
