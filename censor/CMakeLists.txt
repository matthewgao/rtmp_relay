set (CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address  -static-libasan -fno-omit-frame-pointer -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -static-libasan")
add_definitions("-Wall")
aux_source_directory(. DIR_LIB_SRCS)

message(STATUS "DIR_SRCS: [${DIR_SRCS}]")

# 添加可执行文件
add_library (censor STATIC ${DIR_LIB_SRCS})