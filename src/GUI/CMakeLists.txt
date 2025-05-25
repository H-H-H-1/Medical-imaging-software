# GUI模块CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 收集GUI模块的源文件
set(GUI_SOURCES
    MainWindow.cpp
    ViewportWidget.cpp
    ParameterPanel.cpp
)

set(GUI_HEADERS
    MainWindow.h
    ViewportWidget.h
    ParameterPanel.h
)

# 创建GUI静态库
add_library(MedicalGUI STATIC ${GUI_SOURCES} ${GUI_HEADERS})

# 设置目标属性
set_target_properties(MedicalGUI PROPERTIES
    AUTOMOC TRUE
    AUTOUIC TRUE
    AUTORCC TRUE
)

# 设置包含目录
target_include_directories(MedicalGUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core
        ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
)

# 链接Qt和其他库
target_link_libraries(MedicalGUI 
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::OpenGL
        MedicalCore
        MedicalUtils
    PRIVATE
        ${VTK_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

# 设置编译特性
target_compile_features(MedicalGUI PRIVATE cxx_std_17)
