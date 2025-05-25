cmake_minimum_required(VERSION 3.16)

project(MedicalImagingSoftware VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(WIN32)
    add_compile_definitions(_USE_MATH_DEFINES)
    if(MSVC)
        add_compile_options(/W3)
        # 添加Qt特定的编译选项
        add_compile_options(/permissive-)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
    endif()
endif()

# 设置Qt和VTK路径
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.0/5.12.0/msvc2017_64")
# 添加VTK路径
list(APPEND CMAKE_PREFIX_PATH "D:/VTK/VTK")

# 查找Qt5 - 添加更详细的组件
find_package(Qt5 REQUIRED COMPONENTS
    Core 
    Widgets 
    Gui 
    OpenGL
    PrintSupport  # 如果需要打印支持
)

# 查找VTK
find_package(VTK REQUIRED)

if(VTK_VERSION VERSION_LESS "8.90.0")
    include(${VTK_USE_FILE})
else()
    message(STATUS "VTK Version: ${VTK_VERSION}")
endif()

# 查找OpenGL
find_package(OpenGL REQUIRED)

# 启用Qt MOC自动处理 - 这很重要！
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置Qt的可执行文件属性（Windows特定）
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# 添加子目录
add_subdirectory(src)

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
