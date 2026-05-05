cmake_minimum_required(VERSION 3.20)
project(reese_pointers C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Strict warnings — good discipline for C
add_compile_options(
    -Wall -Wextra -Wpedantic
    -Wno-unused-parameter   # relax this one for stubs
    -fsanitize=address,undefined
)
add_link_options(-fsanitize=address,undefined)

# Find Criterion (installed via Homebrew)
find_library(CRITERION_LIB criterion REQUIRED
    HINTS /opt/homebrew/lib)
find_path(CRITERION_INC criterion/criterion.h
    HINTS /opt/homebrew/include)

# Helper function to reduce boilerplate per chapter
function(add_chapter_tests chapter)
    file(GLOB TEST_SRCS tests/${chapter}/*.c)
    file(GLOB SRC_SRCS  src/${chapter}/*.c)
    foreach(TEST_SRC ${TEST_SRCS})
        get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SRC} ${SRC_SRCS})
        target_include_directories(${TEST_NAME} PRIVATE
            src/${chapter} ${CRITERION_INC})
        target_link_libraries(${TEST_NAME} ${CRITERION_LIB})
    endforeach()
endfunction()

add_chapter_tests(hello)
add_chapter_tests(chapter01)
add_chapter_tests(chapter02)
add_chapter_tests(chapter03)
add_chapter_tests(chapter04)
add_chapter_tests(chapter05)
add_chapter_tests(chapter06)
add_chapter_tests(chapter07)