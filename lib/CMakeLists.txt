cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(libkbd_monitor VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)


set(header_interface
    include/kbd_monitor/KeyboardMonitor.hpp
)

set(src
    src/KeyboardMonitor.cpp
    src/FileDescriptor.cpp
)


add_library(kbd_monitor
    ${header_interface}
    ${src}
)

add_library(KbdMonitor::kbd_monitor ALIAS kbd_monitor)

target_include_directories(kbd_monitor
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(kbd_monitor PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(kbd_monitor PRIVATE
    Threads::Threads
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/KbdMonitor)

install(TARGETS kbd_monitor
    EXPORT kbd_monitor-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(kbd_monitor PROPERTIES EXPORT_NAME KbdMonitor)


message(TRACE "========= Finish configure: ${PROJECT_NAME}")