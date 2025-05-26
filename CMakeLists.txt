cmake_minimum_required(VERSION 3.16)

project(MedicalImagingSoftware VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(WIN32)
    add_compile_definitions(_USE_MATH_DEFINES)
    if(MSVC)
        add_compile_options(/W3 /permissive- /Zc:__cplusplus)
    endif()
endif()

# 设置Qt和VTK的路径 (根据您的实际安装路径修改)
set(CMAKE_PREFIX_PATH
    "C:/Qt/Qt5.12.0/5.12.0/msvc2017_64"  # Qt路径
    "D:/VTK/VTK"                         # VTK路径
)

# 启用Qt MOC, UIC, RCC自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找Qt5
find_package(Qt5 REQUIRED COMPONENTS
    Core 
    Widgets 
    Gui
    OpenGL
)

# 查找VTK
find_package(VTK REQUIRED COMPONENTS
    RenderingOpenGL2
    InteractionStyle
    IOImage
    ImagingCore
    CommonCore
    # 根据需要添加更多VTK组件
)
include(${VTK_USE_FILE})

# 查找OpenGL
find_package(OpenGL REQUIRED)

# 添加子目录 (src目录)
add_subdirectory(src)

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# 如果需要安装Qt DLL文件（调试时很有用）
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 添加Qt bin目录到PATH，以便运行时能找到DLL
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
endif()

# 配置信息
message(STATUS "Building Medical Imaging Software")
message(STATUS "Qt5 version: ${Qt5_VERSION}")
message(STATUS "VTK version: ${VTK_VERSION}")
