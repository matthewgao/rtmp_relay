cmake_minimum_required(VERSION 3.8)

# 设置项目名称
project(RTMPRelay)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 17)

add_definitions("-Wall")

add_subdirectory(processor)

aux_source_directory(. DIR_SRCS)
# find_package(FFmpeg REQUIRED)

# this is not necessary if you have installed FFmpeg in a standard path
# set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/5.0.1/include")
# set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/5.0.1/lib") 
set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/include")
set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/lib") 

if (UNIX)
    SET(CMAKE_CXX_FLAGS "-pthread")
endif (UNIX)

include_directories(${FFMPEG_INCLUDE_DIR})
include_directories("${CMAKE_CURRENT_BINARY_DIR}/processor")
include_directories(.)

link_directories(${FFMPEG_LIBRARY_DIR})

find_library(AVFORMAT_LIBRARY NAMES avformat PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVCODEC_LIBRARY NAMES avcodec PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVUTIL_LIBRARY NAMES avutil PATHS ${FFMPEG_LIBRARY_DIR})

# 添加可执行文件
add_executable(rtmp_relay ${DIR_SRCS})
target_link_libraries(rtmp_relay 
    PRIVATE
    processor
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
)
