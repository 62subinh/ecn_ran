cmake_minimum_required(VERSION 3.22)
project(oai_l4s_helper)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#find_package(ZeroMQ)
#find_package(catkin REQUIRED COMPONENTS roscpp)

#include_directories(include ${ZMQ_INCLUDE_DIRS})


set(OAIL4S_DIR  ${CMAKE_SOURCE_DIR})
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(kernel REQUIRED)

set(MARKER_DIR  ${OAIL4S_DIR}/marker)
set(TEST_DIR    ${OAIL4S_DIR}/test)

set(BPF_INCLUDE ${OAIL4S_DIR}/include)
set(BPF_LIB     ${OAIL4S_DIR}/lib/libbpf.a)
add_subdirectory("${MARKER_DIR}")
add_subdirectory("${TEST_DIR}")

#include_directories("${OAIL4S_DIR}/include")
#link_directories("${OAIL4S_DIR}/lib")

add_executable(oai_ecn_marker ${MARKER_DIR}/marker.c)
#add_executable(test_talker ${TEST_DIR}/talker.c)
#add_executable(test_listener ${TEST_DIR}/listener.c)

target_link_libraries(oai_ecn_marker
    PRIVATE 
    "{BPF_LIB}")
target_include_directories(oai_ecn_marker
    PRIVATE
        "${BPF_INCLUDE}"
        "${KERNEL_INCLUDE}"
)
