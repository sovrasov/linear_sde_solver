cmake_minimum_required(VERSION 3.5)

add_subdirectory(3rd-party/dcmt0.6.2)

set(PROJECT_NAME_STR linear_sde_integrator)
project(${PROJECT_NAME_STR})

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/core/include/*.hpp)

add_library(${PROJECT_NAME_STR} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME_STR} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME_STR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME_STR} dcmt)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(${PROJECT_NAME_STR} INTERFACE -Wall -fopenmp -march=native)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_definitions(/EHsc /openmp)
endif()

add_executable(sde_sample ${CMAKE_CURRENT_SOURCE_DIR}/sample_src/main.cpp)
target_link_libraries(sde_sample ${PROJECT_NAME_STR})
