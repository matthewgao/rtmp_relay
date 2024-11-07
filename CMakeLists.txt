cmake_minimum_required(VERSION 3.8)

# 设置项目名称
project(RTMPRelay)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 17)
# set(CMAKE_C_COMPILER "/usr/local/gcc-5.4.0/bin/gcc")
# set(CMAKE_CXX_COMPILER "/usr/local/gcc-5.4.0/bin/g++")


add_definitions("-Wall")
# if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
# # enable asan only for debug
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         message(STATUS "Enabling ASan for Debug builds")
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address  -g")
#         set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address ")
        
#         # link_directories("/usr/local/gcc-5.4.0/lib64/")
#     endif()
# endif()


add_subdirectory(processor)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -static-libasan -fno-omit-frame-pointer  -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -static-libasan ")
aux_source_directory(. DIR_SRCS)
# find_package(FFmpeg REQUIRED)

# this is not necessary if you have installed FFmpeg in a standard path
# set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/5.0.1/include")
# set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/5.0.1/lib") 
# set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/include")
# set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/lib") 
# include_directories(${FFMPEG_INCLUDE_DIR})
# link_directories(${FFMPEG_LIBRARY_DIR})

if (UNIX)
    SET(CMAKE_CXX_FLAGS "-pthread")
endif (UNIX)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/processor")
include_directories(.)

find_library(AVFORMAT_LIBRARY NAMES avformat PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVCODEC_LIBRARY NAMES avcodec PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVUTIL_LIBRARY NAMES avutil PATHS ${FFMPEG_LIBRARY_DIR})
find_library(SWR_LIBRARY NAMES swresample PATHS ${FFMPEG_LIBRARY_DIR})

# 添加可执行文件
add_executable(rtmp_relay ${DIR_SRCS})
target_link_libraries(rtmp_relay 
    PRIVATE
    processor
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${SWR_LIBRARY}
    z
    fdk-aac
)

# target_link_libraries(rtmp_relay 
#     PRIVATE
#     processor
#     ${AVFORMAT_LIBRARY}
#     ${AVCODEC_LIBRARY}
#     ${AVUTIL_LIBRARY}
#     asan
#     z
#     fdk-aac
# )
