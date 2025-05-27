====================================================================
                     MEDICAL IMAGING SOFTWARE
====================================================================

PROJECT OVERVIEW
================

This is a modular medical imaging software built with Qt5 and VTK 
(Visualization Toolkit). The software provides comprehensive capabilities 
for medical image processing, visualization, and analysis with a 
user-friendly graphical interface.

ARCHITECTURE
============

The project follows a modular architecture with the following components:

Core Modules:

1. Core Module (src/Core/)
   - DataModel: Handles medical image data structures and management
   - RenderingEngine: VTK-based 3D rendering and visualization engine
   - ImageProcessor: Advanced image processing algorithms and filters
   - RegistrationManager: Image registration and alignment capabilities

2. GUI Module (src/GUI/)
   - MainWindow: Primary application window with menu and toolbar
   - ViewportWidget: 3D viewport for image visualization and interaction
   - ParameterPanel: Control panel for adjusting processing parameters

3. Utils Module (src/Utils/)
   - Config: Configuration management and settings
   - Logger: Logging system for debugging and monitoring
   - VTKUtils: VTK utility functions and helper classes

4. Plugins Module (src/Plugins/)
   - PluginManager: Dynamic plugin loading and management
   - PluginInterface: Abstract interface for extending functionality

FEATURES
========

- Medical Image Loading: Support for DICOM and other medical image formats
- 3D Visualization: Interactive 3D rendering with VTK
- Image Processing: Advanced filtering and enhancement algorithms
- Image Registration: Automated alignment of multiple image datasets
- Plugin System: Extensible architecture for custom functionality
- User-Friendly Interface: Intuitive Qt-based graphical interface

DEPENDENCIES
============

Required Libraries:

Qt 5.12.0 (msvc2017_64)
- Qt5::Core
- Qt5::Widgets  
- Qt5::Gui
- Qt5::OpenGL (if applicable)

VTK 9.3+
- VTK rendering components
- VTK image processing modules
- VTK interaction widgets

Compiler Requirements:
- Visual Studio 2019 or later
- MSVC v142 toolset
- C++14 or later standard

BUILD INSTRUCTIONS
==================

Prerequisites:
1. Install Qt 5.12.0 with MSVC 2017 64-bit compiler
2. Install or build VTK 9.3+ with Qt support
3. Install Visual Studio 2019 with C++ development tools
4. Install CMake 3.16 or later

Configuration:
Set the following environment paths in CMakeLists.txt:

CMAKE_PREFIX_PATH should include:
- "C:/Qt/5.12.0/msvc2017_64"
- "C:/VTK/lib/cmake/vtk-9.3"

Build Steps:

1. Clone the repository
   git clone [repository-url]
   cd "Medical imaging software"

2. Create build directory
   mkdir build
   cd build

3. Configure with CMake
   cmake .. -G "Visual Studio 16 2019" -A x64

4. Build the project
   cmake --build . --config Release

   Or open the generated Visual Studio solution file and build from IDE.

PROJECT STRUCTURE
==================

Medical imaging software/
├── CMakeLists.txt              # Main CMake configuration
├── CMakePresets.json           # CMake presets for configurations
├── README.md                   # Markdown documentation
├── README.txt                  # This text documentation
├── build/                      # Build output directory
│   ├── src/Core/              # Core library outputs
│   ├── src/GUI/               # GUI library outputs  
│   ├── src/Utils/             # Utils library outputs
│   └── src/Plugins/           # Plugins library outputs
└── src/                       # Source code directory
    ├── CMakeLists.txt         # Source CMake configuration
    ├── main.cpp               # Application entry point
    ├── Core/                  # Core functionality module
    │   ├── CMakeLists.txt
    │   ├── DataModel.cpp/.h
    │   ├── RenderingEngine.cpp/.h
    │   ├── ImageProcessor.cpp/.h
    │   └── RegistrationManager.cpp/.h
    ├── GUI/                   # User interface module
    │   ├── CMakeLists.txt
    │   ├── MainWindow.cpp/.h/.ui
    │   ├── ViewportWidget.cpp/.h
    │   └── ParameterPanel.cpp/.h/.ui
    ├── Utils/                 # Utility functions module
    │   ├── CMakeLists.txt
    │   ├── Config.cpp/.h
    │   ├── Logger.cpp/.h
    │   └── VTKUtils_fixed.cpp/.h
    └── Plugins/               # Plugin system module
        ├── CMakeLists.txt
        ├── PluginManager.cpp/.h
        └── PluginInterface.h

BUILD STATUS
============

Successfully Built Components:

✓ Utils Library (MedicalUtils.lib)
  - Configuration management
  - Logging system  
  - VTK utilities

✓ Core Library (MedicalCore.lib)
  - Data model implementation
  - Rendering engine
  - Image processing algorithms
  - Registration manager

✓ Plugins Library (MedicalPlugins.lib)
  - Plugin management system
  - Plugin interface definitions

Pending Components:

⏳ GUI Library (MedicalGUI.lib)
   - Currently experiencing compilation issues
   - MainWindow.cpp requires additional fixes

⏳ Main Executable (MedicalImaging.exe)
   - Waiting for GUI library completion
   - DLL dependencies need resolution

USAGE
=====

Once built successfully, the application can be launched from:
build/src/Release/MedicalImaging.exe

Basic Workflow:

1. Launch Application: Start the medical imaging software
2. Load Images: Use File menu to load DICOM or other medical images
3. Visualize: View images in the 3D viewport
4. Process: Apply image processing algorithms via parameter panel
5. Register: Align multiple image datasets if needed
6. Export: Save processed results

DEVELOPMENT
===========

Adding New Features:

1. Core Functionality: Add new classes to the Core module
2. GUI Components: Extend the GUI module with new widgets
3. Utilities: Add helper functions to the Utils module
4. Plugins: Create new plugins implementing PluginInterface

Code Style:

- Follow Qt coding conventions
- Use VTK smart pointers for VTK objects
- Implement proper error handling and logging
- Maintain modular design principles

TROUBLESHOOTING
===============

Common Build Issues:

1. Qt Path Issues: Verify Qt installation path in CMakeLists.txt
2. VTK Compatibility: Ensure VTK version 9.3+ with Qt support
3. Compiler Mismatch: Use consistent MSVC toolset across dependencies
4. Missing Dependencies: Check all required libraries are installed

Runtime Issues:

1. DLL Not Found: Ensure Qt and VTK DLLs are in PATH or app directory
2. OpenGL Issues: Verify graphics drivers support required OpenGL version
3. Memory Issues: Monitor memory usage for large datasets

CURRENT PROJECT STATUS
======================

The project is approximately 75% complete:

- Core infrastructure: COMPLETED
- Library modules: 3 of 4 COMPLETED (Utils, Core, Plugins)
- GUI module: IN PROGRESS (compilation issues being resolved)
- Main executable: PENDING (waiting for GUI completion)

Recent Changes Made:

1. CMakeLists.txt Updates:
   - Set CMAKE_PREFIX_PATH for Qt and VTK
   - Added proper library linking
   - Enabled AUTOMOC, AUTOUIC, AUTORCC

2. Code Fixes:
   - Fixed MainWindow.cpp encoding and QMessageBox issues
   - Resolved ParameterPanel.cpp slot declarations
   - Corrected main.cpp AutoMoc errors
   - Added missing includes and headers

3. Build Configuration:
   - Successfully configured with Visual Studio 2019 generator
   - Built 3 core libraries successfully
   - GUI module compilation in progress

Next Steps:
1. Complete GUI module compilation fixes
2. Build final executable with proper DLL linking
3. Perform runtime testing and verification

====================================================================
Last Updated: Current Build Session
Project Status: Core modules completed, GUI module in progress
====================================================================
