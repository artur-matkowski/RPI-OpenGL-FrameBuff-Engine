# RPI-OpenGL-FrameBuff-Engine (ASAPI)

A cross-platform OpenGL rendering engine designed for Raspberry Pi and desktop environments. The engine features a component-based architecture with an integrated ImGui editor, supporting both OpenGL ES 2.0 (embedded systems) and OpenGL 2.1 (desktop).

## Features

- **Cross-Platform Context Support**
  - DRM/GBM/EGL for framebuffer rendering on Raspberry Pi (no X11 required)
  - GLFW/EGL for desktop development and testing
  - Automatic platform detection and configuration

- **Component-Based Architecture**
  - Entity-Component System (ECS) design
  - Modular systems: Renderer, Resources, Materials, Scene management
  - Serializable game objects with JSON support

- **Integrated Editor**
  - ImGui-based visual editor (ASAPI Studio)
  - Hierarchy window for scene management
  - Real-time statistics and profiling
  - Editor and player build configurations

- **Resource Management**
  - Smart resource tracking and caching
  - Support for multiple formats: FBX meshes, PNG textures, GLSL shaders
  - Binary resource tracking system
  - Automatic resource loading and unloading

- **Material System**
  - Shader-based material pipeline
  - Uniform management and material instances
  - Custom material definitions

## Project Structure

```
RPI-OpenGL-FrameBuff-Engine/
├── src/
│   ├── Contexts/        # Platform-specific rendering contexts
│   │   ├── DRM/         # Direct Rendering Manager (Raspberry Pi)
│   │   ├── GLFW/        # GLFW context (Desktop)
│   │   └── base/        # Base context interfaces
│   ├── Core/            # Core engine objects and serialization
│   ├── Entities/        # Game objects and components
│   │   └── Components/  # Component implementations
│   ├── Systems/         # Engine systems
│   │   ├── Renderer/    # Rendering system
│   │   ├── Resources/   # Resource management
│   │   ├── Materials/   # Material system
│   │   ├── Studio/      # Editor implementation
│   │   └── Memory/      # Memory management
│   └── DesignPatterns/  # Utility patterns and profiling
├── vendor/              # Third-party dependencies (submodules)
│   ├── imgui/           # Dear ImGui
│   ├── glm/             # OpenGL Mathematics
│   ├── ImGuiFileDialog/ # File dialog for ImGui
│   ├── hash-library/    # Hash utilities
│   └── bitforgeutils/   # Custom utilities
├── _exampleProject/     # Example project with assets
└── docs/                # Generated documentation
```

## Prerequisites

### System Dependencies

The engine requires the following system packages:

```bash
sudo apt update
sudo apt install g++ gdb libgbm-dev libgles2-mesa-dev libassimp-dev \
                 assimp-utils libpng-dev libglfw3 libglfw3-dev libdrm-dev
```

### Platform-Specific Requirements

**For Desktop Development:**
- NVIDIA drivers (nouveau may not work reliably)
- GLFW3 library

**For Raspberry Pi:**
- Direct Rendering Manager (DRM) support
- GBM (Generic Buffer Management)
- OpenGL ES 2.0 drivers

**Note:** On Raspberry Pi, you may need to adjust the [R-Pi config](https://elinux.org/R-Pi_configuration_file) file to disable conflicting KMS drivers (`vc4-kms-v3d` and `vc4-fkms-v3d`) if you encounter "Failed to add service - already in use?" errors. See the [Troubleshooting](#troubleshooting) section for details.

## Building

### Quick Start (Recommended)

1. **Configure dependencies:**
   ```bash
   ./configure.sh
   ```

2. **Build the editor:**
   ```bash
   make build-easy
   ```
   This will build all dependencies and the debug editor.

3. **Run the editor:**
   ```bash
   make run
   ```

### Build Targets

The Makefile provides several build configurations:

```bash
# Build everything (dependencies, debug/release editor and player)
make all

# Build only dependencies
make lib

# Debug builds
make debug-editor    # Editor with debug symbols
make debug-player    # Player runtime with debug symbols

# Release builds
make release-editor  # Optimized editor build
make release-player  # Optimized player runtime

# Clean build artifacts
make clean
```

### Build Output Locations

- Debug Editor: `build/dbg/editor/asapi`
- Release Editor: `build/rel/editor/asapi`
- Debug Player: `build/dbg/player/asapi`
- Release Player: `build/rel/player/asapi`

## Usage

### Running the Editor

1. Launch the editor:
   ```bash
   ./build/dbg/editor/asapi
   ```

2. Open a project:
   - When the editor starts, use the file dialog to open a project
   - Navigate to the `_exampleProject` directory
   - Click OK

3. Interface Overview:
   - **Hierarchy Window:** Scene object tree
   - **Editor Window:** Main viewport
   - **Stats Window:** Performance metrics
   - **ASAPI Studio:** Engine settings

   *Note: ImGui windows may open minimized initially. Look for small window headers that can be expanded.*

### Example Project

The included example project (`_exampleProject/`) demonstrates:
- Basic mesh rendering (tilted rectangle)
- Material and shader usage
- Texture mapping
- Scene structure

Project structure:
```
_exampleProject/
├── assets_ext/
│   ├── meshes/      # FBX models
│   ├── shaders/     # GLSL vertex and fragment shaders
│   ├── textures/    # PNG textures
│   └── materials/   # Material definitions (.mat)
└── json/            # Scene and object serialization
```

## Configuration

### Build Flags

The engine uses preprocessor definitions to control features:

- `IS_EDITOR` - Enable editor features (ImGui, file dialog)
- `IS_PLAYER` - Player runtime mode
- `IS_TARGET` - Target device build (Raspberry Pi)
- `LOG_LEVEL` - Logging verbosity (DEBUG, INFO, etc.)
- `IMGUI_IMPL_OPENGL_ES2` - Use OpenGL ES 2.0 implementation

### Architecture Detection

The build system automatically detects the target architecture:
- `armhf` - ARM (Raspberry Pi) - builds with DRM/GBM context
- `amd64`/`x86_64` - Desktop - builds with GLFW context

## Troubleshooting

### "Failed to add service - already in use?"

This error occurs on Raspberry Pi when the KMS drivers conflict with direct framebuffer access.

**Solution:**
1. Edit `/boot/config.txt` (or `/boot/firmware/config.txt` on newer systems)
2. Comment out or remove these lines:
   ```
   #dtoverlay=vc4-kms-v3d
   #dtoverlay=vc4-fkms-v3d
   ```
3. Reboot the system

**References:**
- [rpi-opengl-without-x Issue #1](https://github.com/matusnovak/rpi-opengl-without-x/issues/1)
- [Stack Overflow Discussion](https://stackoverflow.com/questions/40490113/eglfs-on-raspberry2-failed-to-add-service-already-in-use)

### Black Screen or No Rendering

- Ensure proper GPU drivers are installed
- For NVIDIA cards, use proprietary drivers (nouveau may not work)
- Check console output for OpenGL errors
- Verify that shader files are present in the project assets

### ImGui Windows Not Visible

ImGui windows open minimized by default. Look for thin window title bars around the edges of the screen and drag to expand them.

### Build Errors

- Run `./configure.sh` to ensure all dependencies are installed
- Check that git submodules are initialized: `git submodule update --init --recursive`
- Verify compiler version supports C++14: `g++ --version`

### Project Path Issues

When opening projects, always use the file dialog to select the correct path. The "last opened" project feature may not work correctly if paths have changed.

## Development

### Dependencies (Git Submodules)

The project uses git submodules for third-party libraries:

```bash
# Initialize submodules
git submodule update --init --recursive

# Update submodules to latest
git submodule update --remote
```

### Code Structure

The engine follows a modular architecture with clear separation of concerns:

- **Contexts** abstract platform-specific rendering
- **Systems** provide core engine functionality
- **Entities/Components** implement game logic
- **Resources** handle asset loading and management

### Adding New Components

1. Create component header/source in `src/Entities/Components/`
2. Inherit from `ComponentBase` or `ComponentInterface`
3. Implement required virtual methods
4. Register in the component system

## Additional Resources

- [OpenGL ES 2.0 on Raspberry Pi (no X11)](https://github.com/matusnovak/rpi-opengl-without-x)
- Generated documentation: `docs/html/index.html`

## License

*License information not specified in the repository.*

## Acknowledgments

This project uses the following open-source libraries:
- [Dear ImGui](https://github.com/ocornut/imgui) - GUI framework
- [GLM](https://github.com/g-truc/glm) - Mathematics library
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog) - File dialog
- [hash-library](https://github.com/stbrumme/hash-library) - Hash utilities
- Assimp - 3D model loading
- libpng - PNG image support
