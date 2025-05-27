# 添加子目录（先构建库）
add_subdirectory(Utils)
add_subdirectory(Core)
add_subdirectory(GUI)
add_subdirectory(Plugins)

# 创建可执行文件
add_executable(MedicalImagingSoftware 
main.cpp
)


# 设置Qt MOC处理
set_target_properties(MedicalImagingSoftware PROPERTIES
    AUTOMOC TRUE
)

# 链接所有模块库
target_link_libraries(MedicalImagingSoftware 
    PRIVATE 
    MedicalCore
    MedicalGUI
    MedicalUtils
    MedicalPlugins
    Qt5::Core 
    Qt5::Widgets 
    Qt5::OpenGL
    ${VTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# 设置编译特性
target_compile_features(MedicalImagingSoftware PRIVATE cxx_std_17)

# 设置包含目录
target_include_directories(MedicalImagingSoftware 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugins
)
