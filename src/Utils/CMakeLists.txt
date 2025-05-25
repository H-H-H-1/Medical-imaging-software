# Utils模块CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 收集Utils模块的源文件
set(UTILS_SOURCES
    Config.cpp
    Logger.cpp
    VTKUtils_fixed.cpp
)

set(UTILS_HEADERS
    Config.h
    Logger.h
    VTKUtils_fixed.h
)

# 创建Utils静态库
add_library(MedicalUtils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})

# 设置目标属性
set_target_properties(MedicalUtils PROPERTIES
    AUTOMOC TRUE
)

# 设置包含目录
target_include_directories(MedicalUtils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接Qt和VTK库
target_link_libraries(MedicalUtils 
    PUBLIC
        Qt5::Core
    PRIVATE
        ${VTK_LIBRARIES}
)

# 设置编译特性
target_compile_features(MedicalUtils PRIVATE cxx_std_17)
