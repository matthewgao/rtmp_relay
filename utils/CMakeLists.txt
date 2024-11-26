set (CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)

add_definitions("-Wall")
aux_source_directory(. DIR_LIB_SRCS)

message(STATUS "DIR_SRCS: [${DIR_SRCS}]")

# 添加可执行文件
add_library (utils STATIC ${DIR_LIB_SRCS})