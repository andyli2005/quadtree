# cmake -S . -B build
# cmake --build build
# convert -resize 600x -colorspace gray -compress none -depth 8 input.jpg output.ppm

cmake_minimum_required(VERSION 3.10)
project(hw3 LANGUAGES C CXX)
option(BUILD_CODEGRADE_TESTS "Build test suites into separate executables" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
include_directories(include)

if (NOT BUILD_CODEGRADE_TESTS)
    add_executable(run_part1_tests tests/src/write_unit_test.c src/image.c)
    target_compile_options(run_part1_tests PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
    target_include_directories(run_part1_tests PUBLIC include tests/include)
    target_link_libraries(run_part1_tests PUBLIC gtest gtest_main pthread m)

    add_executable(run_part2_tests tests/src/part2_tests.c src/image.c src/qtree.c)
    target_compile_options(run_part2_tests PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
    target_include_directories(run_part2_tests PUBLIC include tests/include)
    target_link_libraries(run_part2_tests PUBLIC gtest gtest_main pthread m)
 endif()

# Build the normal executable. Suitable for use with Valgrind.
add_executable(hw3_main src/qtree.c src/image.c src/hw3_main.c tests/src/tests_utils.c)
target_compile_options(hw3_main PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_include_directories(hw3_main PUBLIC include tests/include)
target_link_libraries(hw3_main PUBLIC m)

# Build an executable with ASAN linked in.
add_executable(hw3_main_asan src/qtree.c src/image.c src/hw3_main.c tests/src/tests_utils.c)
target_compile_options(hw3_main_asan PUBLIC -g -fsanitize=address -fsanitize=leak -fsanitize=undefined -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_options(hw3_main_asan PUBLIC -fsanitize=address -fsanitize=leak -fsanitize=undefined)
target_include_directories(hw3_main_asan PUBLIC include tests/include)
target_link_libraries(hw3_main_asan PUBLIC m asan)
