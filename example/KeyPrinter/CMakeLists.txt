cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(keyPrinter VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(src
    main.cpp
)

add_executable(keyPrinter
    ${src}
)

target_compile_options(keyPrinter PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(keyPrinter
    PRIVATE
    KbdMonitor::kbd_monitor
)

message(TRACE "========= Finish configure: ${PROJECT_NAME}")