# Cellular Threshold

An educational game engine combining AAA-quality technical architecture with cellular biology themes. Players engage with immunology and cellular biology concepts through gameplay mechanics, using immune cell types as characters and biological tissues as environments.

## Requirements

### System Dependencies

| Component | Version | Purpose |
|-----------|---------|---------|
| C++ Compiler | MSVC 2022 / GCC 11+ / Clang 14+ | C++20 support required |
| CMake | 3.20+ | Build system |
| Vulkan SDK | 1.3+ | Graphics API and validation layers |
| Git | 2.0+ | Submodule management |

### Platform-Specific Setup

**Windows (MSVC 2022)**

Install via Visual Studio Installer:
- Desktop development with C++
- Game development with C++
- MSVC v143 build tools
- Windows 11 SDK (10.0.22621.0+)
- C++ CMake tools

**Linux (Ubuntu 22.04+)**

```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
sudo apt install libwayland-dev libxkbcommon-dev
```

### Vulkan SDK Installation

Download from [LunarG](https://vulkan.lunarg.com/sdk/home).

Verify installation:
```bash
vulkaninfo --summary
vkcube
```

Environment variables (add to shell profile):
```bash
export VULKAN_SDK=/path/to/vulkan/sdk
export VK_LAYER_PATH=$VULKAN_SDK/etc/vulkan/explicit_layer.d
export PATH=$VULKAN_SDK/bin:$PATH
```

## Building

### Clone with Submodules

```bash
git clone --recursive https://github.com/smith6jt-cop/OMICS.git
cd OMICS
```

If already cloned without `--recursive`:
```bash
git submodule update --init --recursive
```

### Configure and Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### Run

```bash
./build/bin/CellularThreshold
```

Expected output (Milestone 1):
```
Selected GPU: [Your GPU Name]
Initialization successful. Running main loop...
```

## Project Structure

```
OMICS/
├── CLAUDE.md                   # Claude Code development instructions
├── CMakeLists.txt              # Root build configuration
├── cmake/
│   ├── CompilerWarnings.cmake  # Warning flags per compiler
│   └── ShaderCompilation.cmake # GLSL→SPIR-V automation
├── src/
│   ├── core/
│   │   ├── engine.cpp/h        # Main engine loop
│   │   ├── window.cpp/h        # GLFW window management
│   │   └── input.cpp/h         # Input handling
│   ├── rendering/
│   │   ├── vulkan_context.cpp/h
│   │   ├── swapchain.cpp/h
│   │   ├── pipeline.cpp/h
│   │   └── multiplex_image/    # Multi-channel biological imaging
│   ├── ecs/                    # Entity Component System
│   ├── asset_pipeline/         # Asset import/processing
│   └── main.cpp
├── shaders/
│   ├── basic.vert
│   └── basic.frag
├── third_party/
│   ├── glfw/                   # Window/input (submodule)
│   └── glm/                    # Math library (submodule)
├── assets/                     # Runtime assets
├── docs/
│   ├── DEVELOPMENT_PHASES.md   # Phase milestones and tasks
│   ├── NARRATIVE.md            # Story and character documentation
│   └── ARCHITECTURE.md         # Technical design documentation
└── scripts/
    └── setup_env.sh            # Environment setup helper
```

## Development

See `docs/DEVELOPMENT_PHASES.md` for the complete development roadmap.

### Current Focus: Phase 1 - Foundation

- [x] Development environment setup
- [x] Project structure and CMake configuration
- [ ] Window creation with GLFW
- [ ] Vulkan context initialization
- [ ] Validation layer integration
- [ ] Swapchain implementation
- [ ] Triangle rendering

## License

[License information]

## Contributing

Development is coordinated through the phase system documented in `docs/DEVELOPMENT_PHASES.md`. When contributing:

1. Check current phase milestone
2. Follow code standards in `CLAUDE.md`
3. Verify builds pass on both Windows and Linux
4. Test against validation milestones
