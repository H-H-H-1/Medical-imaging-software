# Core模块CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 收集Core模块的源文件
set(CORE_SOURCES
    DataModel.cpp
    RenderingEngine.cpp
    ImageProcessor.cpp
    RegistrationManager.cpp
)

set(CORE_HEADERS
    DataModel.h
    RenderingEngine.h
    ImageProcessor.h
    RegistrationManager.h
)

# 创建Core静态库
add_library(MedicalCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# 设置目标属性
set_target_properties(MedicalCore PROPERTIES
    AUTOMOC TRUE
)

# 设置包含目录
target_include_directories(MedicalCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
)

# 链接Qt和VTK库
target_link_libraries(MedicalCore 
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        MedicalUtils
    PRIVATE
        ${VTK_LIBRARIES}
)

# 设置编译特性
target_compile_features(MedicalCore PRIVATE cxx_std_17)
