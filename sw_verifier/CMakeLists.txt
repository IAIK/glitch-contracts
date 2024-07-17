cmake_minimum_required(VERSION 3.10)
project(powercontracts VERSION "0.1.0")
set(CMAKE_CXX_STANDARD 20)

set(DEFAULT_BUILD_TYPE Debug)
set(DEFAULT_C_FLAGS_GCC   "-Wall -Wextra -pedantic")
set(DEFAULT_CXX_FLAGS_GCC "-Wall -Wextra -pedantic -fno-rtti")

include(AssureOutOfSourceBuilds.cmake)
AssureOutOfSourceBuilds()

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

add_compile_options (-g -gdwarf-4)
add_compile_options (-O3)
#add_compile_options (-Og)
#add_compile_options (-fsanitize=address -fno-omit-frame-pointer)
#add_compile_options (-fsanitize-address-use-after-return=always -fsanitize-address-use-after-scope)
#add_link_options (-fsanitize=address)
link_libraries(profiler)

add_subdirectory(external)
#add_subdirectory(simulator-concrete)
add_subdirectory(simulator-symbolic)
add_subdirectory(smtgen-cbmc)
#add_subdirectory(test)
