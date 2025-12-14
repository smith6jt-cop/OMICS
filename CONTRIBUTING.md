# Contributing to Cellular Threshold

Thank you for your interest in contributing to the Cellular Threshold engine! This document provides guidelines and information for contributors.

## Table of Contents

- [Branch Strategy](#branch-strategy)
- [Development Workflow](#development-workflow)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Code Standards](#code-standards)
- [Release Process](#release-process)

---

## Branch Strategy

We use a modified Git Flow branching model optimized for game engine development.

### Branch Types

| Branch | Purpose | Naming | Lifetime |
|--------|---------|--------|----------|
| `main` | Stable releases only | `main` | Permanent |
| `develop` | Integration branch | `develop` | Permanent |
| `feature/*` | New features | `feature/feature-name` | Temporary |
| `bugfix/*` | Bug fixes for develop | `bugfix/issue-description` | Temporary |
| `release/*` | Release preparation | `release/X.Y.Z` | Temporary |
| `hotfix/*` | Urgent production fixes | `hotfix/X.Y.Z` | Temporary |

### Branch Diagram

```
main ─────●─────────────────────●─────────────────●──────
          │                     │                 │
          │    release/1.0.0    │   hotfix/1.0.1  │
          │    ┌────●────┐      │   ┌────●────┐   │
          │    │         │      │   │         │   │
develop ──●────●─────────●──────●───●─────────●───●──────
          │    │                    │
          │    └── feature/window-system
          │
          └── feature/vulkan-context
```

### Branch Descriptions

#### `main`
- Contains only production-ready code
- Every commit is a tagged release
- Protected: requires PR with approvals
- Direct pushes prohibited

#### `develop`
- Primary integration branch
- All features merge here first
- Should always be buildable
- Protected: requires PR

#### `feature/*`
- Created from: `develop`
- Merges into: `develop`
- Naming: `feature/short-description` or `feature/ISSUE-123-description`
- Delete after merging

```bash
# Create feature branch
git checkout develop
git pull origin develop
git checkout -b feature/entity-system

# Work on feature...
git push -u origin feature/entity-system

# Create PR to develop when ready
```

#### `bugfix/*`
- Created from: `develop`
- Merges into: `develop`
- For non-critical bugs discovered in develop
- Naming: `bugfix/issue-description`

```bash
git checkout develop
git checkout -b bugfix/memory-leak-in-renderer
```

#### `release/*`
- Created from: `develop`
- Merges into: `main` AND `develop`
- Only bug fixes, documentation, and version bumps
- No new features allowed

```bash
# Start release
git checkout develop
git checkout -b release/1.0.0

# Update version, fix release bugs
# When ready, merge to main and develop
git checkout main
git merge --no-ff release/1.0.0
git tag -a v1.0.0 -m "Release 1.0.0"

git checkout develop
git merge --no-ff release/1.0.0

# Delete release branch
git branch -d release/1.0.0
```

#### `hotfix/*`
- Created from: `main`
- Merges into: `main` AND `develop`
- For critical production bugs only
- Naming: `hotfix/X.Y.Z` (next patch version)

```bash
# Critical bug in production
git checkout main
git checkout -b hotfix/1.0.1

# Fix the bug
git commit -m "fix: resolve crash on startup"

# Merge to both branches
git checkout main
git merge --no-ff hotfix/1.0.1
git tag -a v1.0.1 -m "Hotfix 1.0.1"

git checkout develop
git merge --no-ff hotfix/1.0.1
```

---

## Development Workflow

### Setting Up Your Environment

1. Fork the repository
2. Clone with submodules:
   ```bash
   git clone --recursive https://github.com/YOUR_USERNAME/OMICS.git
   cd OMICS
   ```

3. Add upstream remote:
   ```bash
   git remote add upstream https://github.com/smith6jt-cop/OMICS.git
   ```

4. Install dependencies (see README.md)

### Making Changes

1. Sync with upstream:
   ```bash
   git fetch upstream
   git checkout develop
   git merge upstream/develop
   ```

2. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. Make changes, following code standards

4. Test locally:
   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ./build/bin/CellularThreshold
   ```

5. Commit using conventional commits

6. Push and create PR

---

## Commit Guidelines

We use [Conventional Commits](https://www.conventionalcommits.org/) for automatic versioning.

### Format

```
<type>(<scope>): <description>

[optional body]

[optional footer(s)]
```

### Types

| Type | Description | Version Bump |
|------|-------------|--------------|
| `feat` | New feature | Minor |
| `fix` | Bug fix | Patch |
| `docs` | Documentation only | None |
| `style` | Code style (formatting) | None |
| `refactor` | Code change (no feature/fix) | None |
| `perf` | Performance improvement | Patch |
| `test` | Adding/updating tests | None |
| `build` | Build system changes | None |
| `ci` | CI configuration | None |
| `chore` | Maintenance tasks | None |

### Scopes

Common scopes for this project:
- `core` - Core engine systems
- `render` - Rendering subsystem
- `vulkan` - Vulkan-specific code
- `window` - Window management
- `ecs` - Entity Component System
- `shader` - Shader code
- `asset` - Asset pipeline
- `build` - Build system

### Examples

```bash
# Feature (minor version bump)
feat(ecs): add component iteration support

# Bug fix (patch version bump)
fix(vulkan): resolve validation error on shutdown

# Breaking change (major version bump)
feat(render)!: redesign material system API

BREAKING CHANGE: Material::bind() now requires a CommandBuffer parameter

# Documentation (no version bump)
docs: update build instructions for Linux

# Multiple scopes
fix(render,shader): correct normal mapping calculation
```

---

## Pull Request Process

### Before Creating a PR

- [ ] Code compiles without warnings
- [ ] All existing tests pass
- [ ] New code has appropriate tests
- [ ] Documentation updated if needed
- [ ] Commits follow conventional format
- [ ] Branch is up to date with target

### PR Title Format

Same as commit format:
```
feat(scope): brief description
```

### PR Description Template

See `.github/PULL_REQUEST_TEMPLATE.md`

### Review Process

1. All PRs require at least one approval
2. CI checks must pass
3. No unresolved conversations
4. Squash merge preferred for features
5. Merge commit for releases/hotfixes

---

## Code Standards

### C++ Guidelines

- C++20 standard
- Use `ct::` namespace
- RAII for resource management
- No raw `new`/`delete`
- Use `[[nodiscard]]` for important return values
- Prefer `std::optional` over sentinel values

### File Organization

- Headers: `.h` extension
- Source: `.cpp` extension
- One class per file (usually)
- Include guards: `#pragma once`

### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| Classes | PascalCase | `VulkanContext` |
| Functions | camelCase | `createBuffer()` |
| Variables | camelCase | `vertexCount` |
| Members | m_camelCase | `m_device` |
| Constants | UPPER_SNAKE | `MAX_FRAMES` |
| Namespaces | lowercase | `ct::` |

### Include Order

```cpp
// 1. Corresponding header (for .cpp files)
#include "rendering/vulkan_context.h"

// 2. Project headers
#include "core/window.h"

// 3. Third-party headers
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

// 4. Standard library
#include <vector>
#include <string>
```

---

## Release Process

### Version Numbers

We use [Semantic Versioning](https://semver.org/):

- **MAJOR**: Breaking API changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, backward compatible

Pre-release versions: `1.0.0-alpha.1`, `1.0.0-beta.2`, `1.0.0-rc.1`

### Automated Releases

1. Commits to `main` trigger version analysis
2. Version bump determined by commit types
3. Tag created automatically
4. Release workflow builds artifacts
5. GitHub Release created with changelog

### Manual Release

For manual control:

```bash
# Create release branch
git checkout develop
git checkout -b release/1.0.0

# Update VERSION file
echo "1.0.0" > VERSION

# Update CMakeLists.txt
# VERSION 1.0.0

# Commit and merge
git commit -am "chore(release): prepare 1.0.0"
# Create PR to main
```

---

## Questions?

- Check existing issues and discussions
- Open a new issue for bugs or features
- Use discussions for questions

Thank you for contributing!
