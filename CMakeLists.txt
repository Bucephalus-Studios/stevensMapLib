cmake_minimum_required(VERSION 3.16)
project(stevensMapLib LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(stevensMathLib
    GIT_REPOSITORY https://github.com/Bucephalus-Studios/stevensMathLib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(stevensMathLib)

add_library(stevensMapLib INTERFACE)
add_library(stevensMapLib::stevensMapLib ALIAS stevensMapLib)

target_include_directories(stevensMapLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(stevensMapLib INTERFACE cxx_std_17)
target_link_libraries(stevensMapLib INTERFACE stevensMathLib)
