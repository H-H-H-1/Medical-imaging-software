# 插件实现子目录CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 这里可以添加具体的插件实现
# 例如：
# add_subdirectory(GaussianFilterPlugin)
# add_subdirectory(VolumeRenderingPlugin)
# add_subdirectory(DistanceMeasurementPlugin)

# 目前创建一个空的目标，避免编译错误
add_custom_target(PluginImplementations
    COMMENT "插件实现目录 - 可在此添加具体插件"
)
