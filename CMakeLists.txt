cmake_minimum_required(VERSION 3.10)
project(image_processor)

# 设置编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14")

add_subdirectory(third_party/pybind11)

# 查找 OpenCV
find_package(OpenCV REQUIRED)
find_package(pybind11 REQUIRED)

# 添加你的源文件
add_library(image_processor SHARED image_processor.cpp MPCM.cpp)

# 使用 pybind11 创建 Python 模块
pybind11_add_module(mpcm MODULE bind_py.cpp)

# 链接 OpenCV 库
target_link_libraries(image_processor ${OpenCV_LIBS})
target_link_libraries(mpcm PRIVATE image_processor ${OpenCV_LIBS})