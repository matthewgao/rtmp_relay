aux_source_directory(. DIR_LIB_SRCS)

set (CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)

message(STATUS "DIR_SRCS: [${DIR_SRCS}]")

find_package(PkgConfig REQUIRED)
# pkg_check_modules(AVFORMAT REQUIRED libavformat)
# pkg_check_modules(AVUTIL REQUIRED libavutil)
# pkg_check_modules(SWRESAMPLE REQUIRED libswresample)
# pkg_check_modules(SWSCALE REQUIRED libswscale)
# pkg_check_modules(AVCODEC REQUIRED libavcodec)

set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/include")
set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/lib") 

include_directories(${FFMPEG_INCLUDE_DIR})
link_directories(${FFMPEG_LIBRARY_DIR})

find_library(AVFORMAT_LIBRARY NAMES avformat PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVCODEC_LIBRARY NAMES avcodec PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVUTIL_LIBRARY NAMES avutil PATHS ${FFMPEG_LIBRARY_DIR})


# 添加可执行文件
add_library (processor STATIC ${DIR_LIB_SRCS})

# 链接 FFmpeg 库
target_link_libraries(processor 
    PRIVATE
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
)