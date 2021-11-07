cmake_minimum_required(VERSION 3.12)

project(imath VERSION 0.0.1)


include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 14)
endif()

option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
    enable_testing()

    add_subdirectory(third_party/Catch2)

    message("Building Tests")
    add_subdirectory(tests)
endif()

add_executable(imath_lib_example
    example.cpp)
target_link_libraries(imath_lib_example PRIVATE project_warnings)
