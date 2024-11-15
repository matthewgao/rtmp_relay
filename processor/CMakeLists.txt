set (CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address  -static-libasan -fno-omit-frame-pointer -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -static-libasan")
# set(CMAKE_C_COMPILER "/usr/local/gcc-5.3.0/bin/gcc")
# set(CMAKE_CXX_COMPILER "/usr/local/gcc-5.3.0/bin/g++")
add_definitions("-Wall")
aux_source_directory(. DIR_LIB_SRCS)

message(STATUS "DIR_SRCS: [${DIR_SRCS}]")

set(FFMPEG_INCLUDE_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/include")
set(FFMPEG_LIBRARY_DIR "/usr/local/Cellar/ffmpeg/6.1.1_7/lib") 
set(ASR_INCLUDE_DIR "./../alibabacloud-nls-cpp-sdk/build/install/NlsSdk3.X_LINUX/include")
set(ASR_LIBRARY_DIR "./../alibabacloud-nls-cpp-sdk/build/install/NlsSdk3.X_LINUX/lib") 

include_directories(${FFMPEG_INCLUDE_DIR})
include_directories(${ASR_INCLUDE_DIR})
include_directories("/root/.local/include")
include_directories("/home/admin/.local/include")
link_directories(${FFMPEG_LIBRARY_DIR})
link_directories(${ASR_LIBRARY_DIR})
link_directories("./../3rd-party/grpc/lib")
include_directories("./../3rd-party/grpc/include")

find_library(AVFORMAT_LIBRARY NAMES avformat PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVCODEC_LIBRARY NAMES avcodec PATHS ${FFMPEG_LIBRARY_DIR})
find_library(AVUTIL_LIBRARY NAMES avutil PATHS ${FFMPEG_LIBRARY_DIR})
find_library(SWR_LIBRARY NAMES swresample PATHS ${FFMPEG_LIBRARY_DIR})
find_library(ASR_LIBRARY NAMES alibabacloud-idst-speech PATHS ${ASR_LIBRARY_DIR})

message(STATUS "AVFORMAT_LIBRARY: [${AVFORMAT_LIBRARY}]")
message(STATUS "AVCODEC_LIBRARY: [${AVCODEC_LIBRARY}]")
message(STATUS "AVUTIL_LIBRARY: [${AVUTIL_LIBRARY}]")
message(STATUS "SWR_LIBRARY: [${SWR_LIBRARY}]")
message(STATUS "ASR_LIBRARY: [${ASR_LIBRARY}]")

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
    censor
)

# target_link_libraries(processor 
#     PRIVATE
#     ${AVFORMAT_LIBRARY}
#     ${AVCODEC_LIBRARY}
#     ${AVUTIL_LIBRARY}
#     ${SWSCALE_LIBRARY}
#     asan
# )