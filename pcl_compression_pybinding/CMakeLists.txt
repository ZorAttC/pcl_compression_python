cmake_minimum_required(VERSION 3.0)
project(Octree_PCC)

# 设置编译类型
set(CMAKE_BUILD_TYPE Release)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_PYTHON_VERSION 3.8)
set(CMAKE_CXX_FLAGS " -Wextra -fPIC")

# 寻找PCL库
find_package(PCL  REQUIRED)
# 查找 pybind11 包
find_package(pybind11 REQUIRED)
# 添加包含目录
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${pybind11_INCLUDE_DIR})

pybind11_add_module(_pcl_compression pcl_compression.cpp)
# 添加PCL库目录
link_directories(${PCL_LIBRARY_DIRS})

# 添加PCL库的定义
add_definitions(${PCL_DEFINITIONS})

target_include_directories(_pcl_compression PRIVATE ${PYBIND11_INCLUDE_DIRS})
target_link_libraries(_pcl_compression PRIVATE ${PCL_LIBRARIES})
