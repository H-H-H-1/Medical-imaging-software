# 医学影像软件

## 项目概述

这是一个基于Qt5和VTK（可视化工具包）构建的模块化医学影像软件。该软件为医学图像处理、可视化和分析提供全面的功能，并配备用户友好的图形界面。

## 架构设计

项目采用模块化架构设计，包含以下组件：

### 核心模块

1. **核心模块** (`src/Core/`)
   - **数据模型（DataModel）**：处理医学图像数据结构和管理
   - **渲染引擎（RenderingEngine）**：基于VTK的3D渲染和可视化引擎
   - **图像处理器（ImageProcessor）**：高级图像处理算法和滤波器
   - **配准管理器（RegistrationManager）**：图像配准和对齐功能

2. **图形界面模块** (`src/GUI/`)
   - **主窗口（MainWindow）**：带有菜单和工具栏的主应用程序窗口
   - **视口组件（ViewportWidget）**：用于图像可视化和交互的3D视口
   - **参数面板（ParameterPanel）**：调整处理参数的控制面板

3. **工具模块** (`src/Utils/`)
   - **配置管理（Config）**：配置管理和设置
   - **日志系统（Logger）**：用于调试和监控的日志系统
   - **VTK工具（VTKUtils）**：VTK实用函数和辅助类

4. **插件模块** (`src/Plugins/`)
   - **插件管理器（PluginManager）**：动态插件加载和管理
   - **插件接口（PluginInterface）**：扩展功能的抽象接口

## 功能特性

- **医学图像加载**：支持DICOM和其他医学图像格式
- **3D可视化**：基于VTK的交互式3D渲染
- **图像处理**：高级滤波和增强算法
- **图像配准**：多图像数据集的自动对齐
- **插件系统**：可扩展的自定义功能架构
- **用户友好界面**：基于Qt的直观图形界面

## 依赖项

### 必需库

- **Qt 5.12.0** (msvc2017_64)
  - Qt5::Core
  - Qt5::Widgets  
  - Qt5::Gui
  - Qt5::OpenGL（如适用）

- **VTK 9.3+**
  - VTK渲染组件
  - VTK图像处理模块
  - VTK交互组件

### 编译器要求

- **Visual Studio 2019** 或更高版本
- **MSVC v142工具集**
- **C++14** 或更高标准

## 构建说明

### 前提条件

1. 安装Qt 5.12.0及MSVC 2017 64位编译器
2. 安装或构建带Qt支持的VTK 9.3+
3. 安装带C++开发工具的Visual Studio 2019
4. 安装CMake 3.16或更高版本

### 环境配置

在CMakeLists.txt中设置以下环境路径：

```cmake
set(CMAKE_PREFIX_PATH 
    "C:/Qt/5.12.0/msvc2017_64"
    "C:/VTK/lib/cmake/vtk-9.3"
)
```

### 构建步骤

1. **克隆仓库**
   ```bash
   git clone [仓库地址]
   cd "Medical imaging software"
   ```

2. **创建构建目录**
   ```bash
   mkdir build
   cd build
   ```

3. **使用CMake配置**
   ```bash
   cmake .. -G "Visual Studio 16 2019" -A x64
   ```

4. **构建项目**
   ```bash
   cmake --build . --config Release
   ```

   或者打开生成的Visual Studio解决方案文件从IDE构建。

## 项目结构

```
Medical imaging software/
├── CMakeLists.txt              # 主CMake配置文件
├── CMakePresets.json           # 不同配置的CMake预设
├── README.md                   # 英文文档
├── README_CN.md                # 中文文档（本文件）
├── build/                      # 构建输出目录
│   ├── src/Core/              # 核心库输出
│   ├── src/GUI/               # GUI库输出  
│   ├── src/Utils/             # 工具库输出
│   └── src/Plugins/           # 插件库输出
└── src/                       # 源代码目录
    ├── CMakeLists.txt         # 源码CMake配置
    ├── main.cpp               # 应用程序入口点
    ├── README.txt             # 文本格式文档
    ├── Core/                  # 核心功能模块
    │   ├── CMakeLists.txt
    │   ├── DataModel.cpp/.h
    │   ├── RenderingEngine.cpp/.h
    │   ├── ImageProcessor.cpp/.h
    │   └── RegistrationManager.cpp/.h
    ├── GUI/                   # 用户界面模块
    │   ├── CMakeLists.txt
    │   ├── MainWindow.cpp/.h/.ui
    │   ├── ViewportWidget.cpp/.h
    │   └── ParameterPanel.cpp/.h/.ui
    ├── Utils/                 # 工具函数模块
    │   ├── CMakeLists.txt
    │   ├── Config.cpp/.h
    │   ├── Logger.cpp/.h
    │   └── VTKUtils_fixed.cpp/.h
    └── Plugins/               # 插件系统模块
        ├── CMakeLists.txt
        ├── PluginManager.cpp/.h
        └── PluginInterface.h
```

## 构建状态

### 成功构建的组件

✅ **工具库** (`MedicalUtils.lib`)
- 配置管理
- 日志系统  
- VTK工具

✅ **核心库** (`MedicalCore.lib`)
- 数据模型实现
- 渲染引擎
- 图像处理算法
- 配准管理器

✅ **插件库** (`MedicalPlugins.lib`)
- 插件管理系统
- 插件接口定义

### 待完成组件

⏳ **GUI库** (`MedicalGUI.lib`)
- 目前存在编译问题
- MainWindow.cpp需要进一步修复

⏳ **主可执行文件** (`MedicalImaging.exe`)
- 等待GUI库构建完成
- DLL依赖需要解决

## 使用说明

构建成功后，应用程序可从以下位置启动：
```
build/src/Release/MedicalImaging.exe
```

### 基本工作流程

1. **启动应用程序**：启动医学影像软件
2. **加载图像**：使用文件菜单加载DICOM或其他医学图像
3. **可视化**：在3D视口中查看图像
4. **处理**：通过参数面板应用图像处理算法
5. **配准**：如需要，对齐多个图像数据集
6. **导出**：保存处理结果

## 开发指南

### 添加新功能

1. **核心功能**：向核心模块添加新类
2. **GUI组件**：使用新组件扩展GUI模块
3. **工具函数**：向工具模块添加辅助函数
4. **插件**：创建实现PluginInterface的新插件

### 代码规范

- 遵循Qt编码约定
- 对VTK对象使用VTK智能指针
- 实现适当的错误处理和日志记录
- 维护模块化设计原则

## 故障排除

### 常见构建问题

1. **Qt路径问题**：验证CMakeLists.txt中的Qt安装路径
2. **VTK兼容性**：确保VTK版本9.3+带Qt支持
3. **编译器不匹配**：在所有依赖项中使用一致的MSVC工具集
4. **缺少依赖项**：检查所有必需库是否已安装

### 运行时问题

1. **找不到DLL**：确保Qt和VTK DLL在PATH中或应用程序目录中
2. **OpenGL问题**：验证图形驱动程序支持所需的OpenGL版本
3. **内存问题**：监控大数据集的内存使用情况

## 当前项目状态

项目大约完成75%：

- **核心基础设施**：已完成
- **库模块**：4个中已完成3个（Utils、Core、Plugins）
- **GUI模块**：进行中（正在解决编译问题）
- **主可执行文件**：待完成（等待GUI完成）

### 最近所做的更改

1. **CMakeLists.txt更新**：
   - 为Qt和VTK设置CMAKE_PREFIX_PATH
   - 添加适当的库链接
   - 启用AUTOMOC、AUTOUIC、AUTORCC

2. **代码修复**：
   - 修复MainWindow.cpp编码和QMessageBox问题
   - 解决ParameterPanel.cpp槽声明问题
   - 纠正main.cpp AutoMoc错误
   - 添加缺失的包含文件和头文件

3. **构建配置**：
   - 成功使用Visual Studio 2019生成器配置
   - 成功构建3个核心库
   - GUI模块编译正在进行中

### 下一步

1. 完成GUI模块编译修复
2. 构建带适当DLL链接的最终可执行文件
3. 执行运行时测试和验证

## 技术特点

### Qt5集成
- 现代化的用户界面设计
- 跨平台兼容性
- 丰富的组件库
- 国际化支持

### VTK可视化
- 强大的3D渲染能力
- 医学图像专用算法
- 交互式可视化
- 高性能计算

### 模块化设计
- 清晰的职责分离
- 易于维护和扩展
- 插件化架构
- 可重用组件

## 许可证

[在此指定您的许可证]

## 贡献

[在此添加贡献指南]

## 联系方式

[在此添加联系信息]

---

*最后更新：2025年5月27日*  
*构建状态：核心模块已完成，GUI模块进行中*
