FILE(GLOB source simple_cpp/accessibility/*.cpp)
add_library(simple-cpp-accessibility-lib ${source})

find_package(PkgConfig REQUIRED)

pkg_check_modules(ATSPI REQUIRED atspi-2)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

target_include_directories(simple-cpp-accessibility-lib PUBLIC
        ${ATSPI_INCLUDE_DIRS}
        ${GOBJECT_INCLUDE_DIRS}
)

target_link_libraries(simple-cpp-accessibility-lib PRIVATE
        ${ATSPI_LIBRARIES}
        ${GOBJECT_LIBRARIES}
)

target_compile_options(simple-cpp-accessibility-lib PRIVATE
        ${ATSPI_CFLAGS_OTHER}
        ${GOBJECT_CFLAGS_OTHER}
)

target_compile_definitions(simple-cpp-accessibility-lib PRIVATE ATSPI_DISABLE_DEPRECATED)
target_compile_features(simple-cpp-accessibility-lib INTERFACE cxx_std_23)
target_include_directories(simple-cpp-accessibility-lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(simple-cpp-accessibility::lib ALIAS simple-cpp-accessibility-lib)