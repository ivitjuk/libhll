cmake_minimum_required (VERSION 2.8)

project (LIBHLL)

add_compile_options (-Wall -Werror -std=c99 -pedantic-errors)

add_library (hll SHARED hll/hll.c hll/hll.h hll/hll_private.h)

target_include_directories (hll PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

