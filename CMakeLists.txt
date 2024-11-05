cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(RTMPRelay)
set(CMAKE_BUILD_TYPE Debug)
# 查找 FFmpeg 库
# find_package(FFmpeg REQUIRED)

# set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/5.0.1/include")
# set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/5.0.1/lib") 

set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/include")
set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/lib") 

include_directories(${FFMPEG_INCLUDE_DIR})
link_directories(${FFMPEG_LIBRARY_DIR})

find_library(AVFORMAT_LIBRARY NAMES avformat PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVCODEC_LIBRARY NAMES avcodec PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVUTIL_LIBRARY NAMES avutil PATHS ${FFMPEG_LIBRARY_DIR})


# 添加可执行文件
add_executable(rtmp_relay main.cpp delayer.cpp replacer.cpp)

# 链接 FFmpeg 库
target_link_libraries(rtmp_relay 
    PRIVATE
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
)
