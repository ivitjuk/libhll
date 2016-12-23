cmake_minimum_required (VERSION 2.8)

project (LIBHLL)

add_compile_options (-O2 -g -Wall -Werror -std=c99 -pedantic-errors)

add_library (hll hll/hll.c hll/hll.h hll/hll_private.h
                 city/city.c city/city.h)
target_include_directories (hll PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(hll_demo hll_demo.c)
target_include_directories (hll_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (hll_demo hll m)

enable_testing()

add_subdirectory(test)

