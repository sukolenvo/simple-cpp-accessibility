cmake_minimum_required(VERSION 3.10)
project(at-spi-decorator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(ATSPI REQUIRED atspi-2)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

find_package(X11 REQUIRED)

add_executable(dump-tree dump-tree.cpp AccessibleObject.cpp
        AccessibleError.cpp)

target_include_directories(dump-tree PRIVATE 
    ${ATSPI_INCLUDE_DIRS} 
    ${GOBJECT_INCLUDE_DIRS}
    ${X11_INCLUDE_DIR}
)

target_link_libraries(dump-tree 
    ${ATSPI_LIBRARIES} 
    ${GOBJECT_LIBRARIES}
    ${X11_LIBRARIES}
)

target_compile_options(dump-tree PRIVATE 
    ${ATSPI_CFLAGS_OTHER} 
    ${GOBJECT_CFLAGS_OTHER}
)

target_compile_definitions(dump-tree PRIVATE ATSPI_DISABLE_DEPRECATED)