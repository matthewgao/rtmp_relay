set (CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address  -static-libasan -fno-omit-frame-pointer -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -static-libasan")
add_definitions("-Wall -g")
aux_source_directory(. DIR_LIB_SRCS)

message(STATUS "DIR_SRCS: [${DIR_SRCS}]")

find_package(PkgConfig REQUIRED)
# pkg_check_modules(AVFORMAT REQUIRED libavformat)
# pkg_check_modules(AVUTIL REQUIRED libavutil)
# pkg_check_modules(SWRESAMPLE REQUIRED libswresample)
# pkg_check_modules(SWSCALE REQUIRED libswscale)
# pkg_check_modules(AVCODEC REQUIRED libavcodec)

set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/include")
set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/lib") 
set(ASR_INCLUDE_DIR "./../alibabacloud-nls-cpp-sdk/build/install/NlsSdk3.X_LINUX/include")
set(ASR_LIBRARY_DIR "./../alibabacloud-nls-cpp-sdk/build/install/NlsSdk3.X_LINUX/lib") 

# set(CMAKE_C_COMPILER "/usr/local/gcc-5.3.0/bin/gcc")
# set(CMAKE_CXX_COMPILER "/usr/local/gcc-5.3.0/bin/g++")



include_directories(${FFMPEG_INCLUDE_DIR})
include_directories(${ASR_INCLUDE_DIR})
link_directories(${FFMPEG_LIBRARY_DIR})
link_directories(${ASR_LIBRARY_DIR})

find_library(AVFORMAT_LIBRARY NAMES avformat PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVCODEC_LIBRARY NAMES avcodec PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVUTIL_LIBRARY NAMES avutil PATHS ${FFMPEG_LIBRARY_DIR})
find_library(SWR_LIBRARY NAMES swresample PATHS ${FFMPEG_LIBRARY_DIR})
find_library(ASR_LIBRARY NAMES alibabacloud-idst-speech PATHS ${ASR_LIBRARY_DIR})


# 添加可执行文件
add_library (processor STATIC ${DIR_LIB_SRCS})

# 链接 FFmpeg 库
target_link_libraries(processor 
    PRIVATE
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${ASR_LIBRARY}
    ${SWR_LIBRARY}
)

# target_link_libraries(processor 
#     PRIVATE
#     ${AVFORMAT_LIBRARY}
#     ${AVCODEC_LIBRARY}
#     ${AVUTIL_LIBRARY}
#     ${SWSCALE_LIBRARY}
#     asan
# )