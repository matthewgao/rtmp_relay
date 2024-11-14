aux_source_directory(. DIR_LIB_SRCS)

set (CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
find_package(PkgConfig REQUIRED)

message(STATUS "PROTOC DIR_SRCS: [${DIR_SRCS}]")

if(GRPC_AS_SUBMODULE)
  # One way to build a projects that uses gRPC is to just include the
  # entire gRPC project tree via "add_subdirectory".
  # This approach is very simple to use, but the are some potential
  # disadvantages:
  # * it includes gRPC's CMakeLists.txt directly into your build script
  #   without and that can make gRPC's internal setting interfere with your
  #   own build.
  # * depending on what's installed on your system, the contents of submodules
  #   in gRPC's third_party/* might need to be available (and there might be
  #   additional prerequisites required to build them). Consider using
  #   the gRPC_*_PROVIDER options to fine-tune the expected behavior.
  #
  # A more robust approach to add dependency on gRPC is using
  # cmake's ExternalProject_Add (see cmake_externalproject/CMakeLists.txt).
  
  # Include the gRPC's cmake build (normally grpc source code would live
  # in a git submodule called "third_party/grpc", but this example lives in
  # the same repository as gRPC sources, so we just look a few directories up)
  # add_subdirectory(../../.. ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
  message(STATUS "Using gRPC via add_subdirectory.")
  
  # After using add_subdirectory, we can now use the grpc targets directly from
  # this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
  set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
else()
  # This branch assumes that gRPC and all its dependencies are already installed
  # on this system, so they can be located by find_package().

  # Find Protobuf installation
  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
  set(protobuf_MODULE_COMPATIBLE TRUE)
  find_package(Protobuf CONFIG REQUIRED)

  message(STATUS "Using protobuf ${protobuf_VERSION}")

  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

  # Find gRPC installation
  # Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
  find_package(gRPC CONFIG REQUIRED)
  message(STATUS "Using gRPC ${gRPC_VERSION}")

  set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Proto file
#get_filename_component(vm_proto "message.proto" ABSOLUTE)
get_filename_component(vm_proto "service.proto" ABSOLUTE)
get_filename_component(vm_proto_path "${vm_proto}" PATH)

message(STATUS "proto dir: [${CMAKE_CURRENT_BINARY_DIR}]")

# Generated sources
set(vm_proto_srcs  "${CMAKE_CURRENT_BINARY_DIR}/service.pb.cc")
set(vm_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.h")
set(vm_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.cc")
set(vm_grpc_hdrs  "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.h")

message(STATUS "vm_proto_srcs: [${vm_proto_srcs}]")

message(STATUS "protoc command: [${_PROTOBUF_PROTOC}]")
add_custom_command(
      OUTPUT "${vm_proto_srcs}" "${vm_proto_hdrs}" "${vm_grpc_srcs}" "${vm_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${vm_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${vm_proto}"
      DEPENDS "${vm_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Proto file
get_filename_component(vm_proto_msg "message.proto" ABSOLUTE)
get_filename_component(vm_proto_msg_path "${vm_proto_msg}" PATH)

message(STATUS "proto dir: [${CMAKE_CURRENT_BINARY_DIR}]")

# Generated sources
set(vm_proto_msg_srcs  "${CMAKE_CURRENT_BINARY_DIR}/message.pb.cc")
set(vm_proto_msg_hdrs "${CMAKE_CURRENT_BINARY_DIR}/message.pb.h")
set(vm_grpc_msg_srcs "${CMAKE_CURRENT_BINARY_DIR}/message.grpc.pb.cc")
set(vm_grpc_msg_hdrs  "${CMAKE_CURRENT_BINARY_DIR}/message.grpc.pb.h")

message(STATUS "vm_proto_msg_srcs: [${vm_proto_msg_srcs}]")

message(STATUS "protoc command: [${_PROTOBUF_PROTOC}]")
add_custom_command(
      OUTPUT "${vm_proto_msg_srcs}" "${vm_proto_msg_hdrs}" "${vm_grpc_msg_srcs}" "${vm_grpc_msg_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${vm_proto_msg_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${vm_proto_msg}"
      DEPENDS "${vm_proto_msg}")

# Include generated *.pb.h files
message(STATUS "proto include_directories: [${CMAKE_CURRENT_BINARY_DIR}]")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("../")
include_directories(../utils)

add_library (grpc STATIC biz_client.cpp ${vm_grpc_srcs} ${vm_proto_srcs} ${vm_grpc_msg_srcs} ${vm_proto_msg_srcs})
target_link_libraries(grpc ${_GRPC_GRPCPP_UNSECURE} ${_PROTOBUF_LIBPROTOBUF})