cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME_STR dcmt)
project (${PROJECT_NAME_STR})

enable_language(C)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
                    ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h
                    ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c)

add_library(${PROJECT_NAME_STR} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME_STR} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME_STR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME_STR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  target_compile_options(${PROJECT_NAME_STR} PRIVATE -std=c99 -pedantic -Werror -Wall -march=native)
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
  add_definitions(/EHsc)
endif()
