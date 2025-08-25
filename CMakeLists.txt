cmake_minimum_required(VERSION 3.22.1)
project(at-spi-decorator)

add_subdirectory(src)

find_package(X11 REQUIRED)

target_include_directories(simple-cpp-accessibility-lib PRIVATE
        ${X11_INCLUDE_DIR}
)

target_link_libraries(simple-cpp-accessibility-lib PRIVATE
        ${X11_LIBRARIES}
)

add_executable(dump-tree dump-tree.cpp)
target_link_libraries(dump-tree PRIVATE simple-cpp-accessibility-lib)
target_compile_features(dump-tree PRIVATE cxx_std_23)
