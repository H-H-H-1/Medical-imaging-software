# Plugins模块CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 收集Plugins模块的源文件
set(PLUGINS_SOURCES
    PluginManager.cpp
)

set(PLUGINS_HEADERS
    PluginInterface.h
    PluginManager.h
)

# 创建Plugins静态库
add_library(MedicalPlugins STATIC ${PLUGINS_SOURCES} ${PLUGINS_HEADERS})

# 设置目标属性
set_target_properties(MedicalPlugins PROPERTIES
    AUTOMOC TRUE
    AUTOUIC TRUE
    AUTORCC TRUE
)

# 设置包含目录
target_include_directories(MedicalPlugins
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core
        ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
)

# 链接Qt和其他库
target_link_libraries(MedicalPlugins 
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        MedicalUtils
    PRIVATE
        ${VTK_LIBRARIES}
)

# 设置编译特性
target_compile_features(MedicalPlugins PRIVATE cxx_std_17)

# 添加实现子目录（如果有具体插件实现）
add_subdirectory(Implementations)