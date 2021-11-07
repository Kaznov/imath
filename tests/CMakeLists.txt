
include(CTest)
include(${CMAKE_SOURCE_DIR}/third_party/Catch2/extras/Catch.cmake)

add_executable(imath_lib_tests
    isPrime.runtime.cpp
    clz.runtime.cpp
    ctz.runtime.cpp
    mod128by64.runtime.cpp
    mul64by64.runtime.cpp)
target_include_directories(imath_lib_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(
    imath_lib_tests
    PRIVATE Catch2::Catch2WithMain project_warnings)

if (TARGET imath_lib_tests)
catch_discover_tests(
    imath_lib_tests
    TEST_PREFIX
    "unittests."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "unittests."
    OUTPUT_SUFFIX
    .xml)
endif()

add_executable(imath_lib_tests_constexpr
    isPrime.constexpr.cpp)
target_include_directories(imath_lib_tests_constexpr PRIVATE ${CMAKE_SOURCE_DIR})
set_property(TARGET imath_lib_tests_constexpr PROPERTY CXX_STANDARD 20)
target_link_libraries(
    imath_lib_tests_constexpr
    PRIVATE Catch2::Catch2WithMain project_warnings)
if(MSVC)
target_compile_options(imath_lib_tests_constexpr PRIVATE  /constexpr:steps100000000)
endif()

if (TARGET imath_lib_tests_constexpr)
catch_discover_tests(
    imath_lib_tests_constexpr
    TEST_PREFIX
    "constexpr."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "constexpr."
    OUTPUT_SUFFIX
    .xml)
endif()
