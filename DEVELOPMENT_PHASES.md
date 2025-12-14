# Development Phases

Cellular Threshold development follows an incremental approach with verifiable milestones at each stage.

## Phase Overview

| Phase | Duration | Focus |
|-------|----------|-------|
| 1 | Weeks 1-2 | Foundation (Window, Vulkan, Triangle) |
| 2 | Weeks 3-4 | Core Systems (Memory, ECS, Logging) |
| 3 | Weeks 5-8 | Rendering Pipeline (Swapchain, Shaders, Materials) |
| 4 | Weeks 9-10 | Multiplex Image System |
| 5 | Weeks 11-14 | Asset Pipeline (Blender integration) |
| 6 | Weeks 15-18 | Game Systems (Physics, Audio) |
| 7 | Weeks 19-24 | Content Pipeline (Biological environments) |

---

## Phase 1: Foundation (Weeks 1-2)

### Objectives

Establish core rendering infrastructure with verified Vulkan pipeline.

### Tasks

#### 1.1 Development Environment

- [ ] Install MSVC 2022 or GCC 11+ with C++20 support
- [ ] Install Vulkan SDK 1.3+ and verify with `vulkaninfo`
- [ ] Configure CMake project structure
- [ ] Initialize Git repository with submodules

#### 1.2 Window System

- [ ] Implement `ct::Window` class with GLFW
- [ ] Configure `WindowConfig` struct (resolution, title, vsync)
- [ ] Handle resize callbacks
- [ ] Implement `createSurface()` for Vulkan integration

#### 1.3 Vulkan Context

- [ ] Create `VkInstance` with validation layers (Debug only)
- [ ] Setup debug messenger for validation output
- [ ] Select physical device with required features
- [ ] Create logical device and queue families
- [ ] Implement RAII cleanup pattern

#### 1.4 Triangle Rendering

- [ ] Implement swapchain creation and recreation
- [ ] Create render pass
- [ ] Build graphics pipeline with basic shaders
- [ ] Implement frame synchronization (semaphores, fences)
- [ ] Render colored triangle

### Validation Milestones

| Milestone | Criteria |
|-----------|----------|
| M1.1 | Window opens, responds to close event |
| M1.2 | GPU name printed to console |
| M1.3 | Validation messages appear (no errors) |
| M1.4 | Colored triangle renders |

### Deliverables

```
src/core/window.cpp, window.h
src/rendering/vulkan_context.cpp, vulkan_context.h
src/rendering/swapchain.cpp, swapchain.h
src/rendering/pipeline.cpp, pipeline.h
src/main.cpp
shaders/basic.vert, basic.frag
```

---

## Phase 2: Core Systems (Weeks 3-4)

### Objectives

Implement foundational engine systems required by all subsystems.

### Tasks

#### 2.1 Memory Management

- [ ] Pool allocator for fixed-size objects
- [ ] Stack allocator for frame-temporary data
- [ ] Vulkan memory allocator wrapper (VMA integration optional)

#### 2.2 Entity Component System

- [ ] `Component` base class
- [ ] `System` base class with `Update()` interface
- [ ] `EntityManager` for entity lifecycle
- [ ] Component storage (sparse set or archetype)

#### 2.3 Logging System

- [ ] Log levels (Trace, Debug, Info, Warning, Error, Fatal)
- [ ] File and console output
- [ ] Timestamped entries
- [ ] Replace `std::cerr` usage

#### 2.4 Configuration

- [ ] JSON or INI configuration loading
- [ ] Runtime settings override
- [ ] Graphics quality presets

### Validation Milestones

| Milestone | Criteria |
|-----------|----------|
| M2.1 | Allocator unit tests pass |
| M2.2 | ECS creates entities, attaches components |
| M2.3 | Logs written to file with correct formatting |

### Deliverables

```
src/core/memory/pool_allocator.cpp, pool_allocator.h
src/core/memory/stack_allocator.cpp, stack_allocator.h
src/ecs/component.h
src/ecs/system.h
src/ecs/entity_manager.cpp, entity_manager.h
src/core/logger.cpp, logger.h
src/core/config.cpp, config.h
```

---

## Phase 3: Rendering Pipeline (Weeks 5-8)

### Objectives

Complete rendering infrastructure for textured meshes.

### Tasks

#### 3.1 Shader System

- [ ] Runtime shader loading from SPIR-V
- [ ] Shader reflection for descriptor layouts
- [ ] Shader hot-reloading (Debug builds)

#### 3.2 Texture System

- [ ] Image loading (stb_image)
- [ ] `VkImage` creation and memory binding
- [ ] Mipmapping generation
- [ ] Sampler management

#### 3.3 Mesh System

- [ ] Vertex buffer abstraction
- [ ] Index buffer support
- [ ] Vertex input descriptions
- [ ] Basic mesh primitives (quad, cube)

#### 3.4 Material System

- [ ] Material definition structure
- [ ] Descriptor set management
- [ ] Uniform buffer updates
- [ ] Material instance support

### Validation Milestones

| Milestone | Criteria |
|-----------|----------|
| M3.1 | Textured quad renders correctly |
| M3.2 | Multiple materials in single frame |
| M3.3 | Shader hot-reload functions |

### Deliverables

```
src/rendering/shader_manager.cpp, shader_manager.h
src/rendering/texture.cpp, texture.h
src/rendering/mesh.cpp, mesh.h
src/rendering/material_system.cpp, material_system.h
src/rendering/descriptor_manager.cpp, descriptor_manager.h
```

---

## Phase 4: Multiplex Image System (Weeks 9-10)

### Objectives

Implement multi-channel biological image processing for cellular visualization.

### Tasks

#### 4.1 Multiplex Loader

- [ ] Load multi-channel TIFF/OME-TIFF images
- [ ] Parse channel metadata (markers, wavelengths)
- [ ] Memory-efficient loading for large images

#### 4.2 Channel Compositor

- [ ] Per-channel color mapping
- [ ] Channel blending modes
- [ ] Intensity normalization
- [ ] Real-time channel toggling

#### 4.3 Texture Array Manager

- [ ] Create Vulkan texture arrays from channel data
- [ ] Manage GPU memory for large channel sets
- [ ] LOD generation for zoom levels

#### 4.4 Atlas Generation

- [ ] Tile large images for streaming
- [ ] Generate texture atlases from cell sprites
- [ ] UV coordinate management

### Validation Milestones

| Milestone | Criteria |
|-----------|----------|
| M4.1 | Load and display 8-channel TIFF |
| M4.2 | Toggle individual channels in real-time |
| M4.3 | Stream large images without memory overflow |

### Deliverables

```
src/rendering/multiplex_image/multiplex_loader.cpp, multiplex_loader.h
src/rendering/multiplex_image/channel_compositor.cpp, channel_compositor.h
src/rendering/multiplex_image/texture_array_manager.cpp, texture_array_manager.h
src/rendering/multiplex_image/atlas_generator.cpp, atlas_generator.h
```

---

## Phase 5: Asset Pipeline (Weeks 11-14)

### Objectives

Integrate professional 3D content creation workflows.

### Tasks

#### 5.1 Asset Importer Framework

- [ ] Generic asset import interface
- [ ] Asset metadata and dependencies
- [ ] Import progress reporting

#### 5.2 Blender Bridge

- [ ] glTF 2.0 import (primary format)
- [ ] FBX import (legacy support)
- [ ] Material conversion
- [ ] Animation data extraction

#### 5.3 Texture Processor

- [ ] Format conversion (PNG/JPG → compressed)
- [ ] Mipmap generation
- [ ] Normal map processing
- [ ] PBR texture packing

#### 5.4 Asset Database

- [ ] Asset registry with UUIDs
- [ ] Dependency tracking
- [ ] Incremental rebuild support
- [ ] Runtime asset streaming

### Validation Milestones

| Milestone | Criteria |
|-----------|----------|
| M5.1 | Import and render Blender glTF model |
| M5.2 | Materials appear correctly |
| M5.3 | Skeletal animation plays |

### Deliverables

```
src/asset_pipeline/asset_importer.cpp, asset_importer.h
src/asset_pipeline/blender_bridge/gltf_importer.cpp, gltf_importer.h
src/asset_pipeline/blender_bridge/fbx_importer.cpp, fbx_importer.h
src/asset_pipeline/blender_bridge/mesh_processor.cpp, mesh_processor.h
src/asset_pipeline/texture_processor.cpp, texture_processor.h
src/asset_pipeline/asset_database.cpp, asset_database.h
```

---

## Phase 6: Game Systems (Weeks 15-18)

### Objectives

Add physics, audio, and gameplay infrastructure.

### Tasks

- [ ] Physics integration (PhysX or Bullet)
- [ ] Collision detection and response
- [ ] Audio system (OpenAL or FMOD)
- [ ] Spatial audio for cellular environments
- [ ] Input action mapping
- [ ] Camera system (cellular navigation)
- [ ] Scene management

---

## Phase 7: Content Pipeline (Weeks 19-24)

### Objectives

Build cellular biology environments and entity systems.

### Tasks

- [ ] Tissue environment generation
- [ ] Cell type entity templates
- [ ] Cytokine signaling visualization
- [ ] Immune response mechanics
- [ ] Educational overlay system

### Target Environments

1. Pancreatic Islets (Islets of Langerhans)
2. Lymph Node Architecture
3. Splenic Tissue (Red/White pulp)
4. Thymic Architecture
5. Systemic Circulation

---

## Progress Tracking

Update this section as phases complete:

```
[x] Phase 1.1 - Environment Setup
[ ] Phase 1.2 - Window System
[ ] Phase 1.3 - Vulkan Context
[ ] Phase 1.4 - Triangle Rendering
[ ] Phase 2 - Core Systems
[ ] Phase 3 - Rendering Pipeline
[ ] Phase 4 - Multiplex Image System
[ ] Phase 5 - Asset Pipeline
[ ] Phase 6 - Game Systems
[ ] Phase 7 - Content Pipeline
```
