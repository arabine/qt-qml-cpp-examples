cmake_minimum_required(VERSION 3.5)

project(test_median LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_median tst_test_median.cpp ../../src/values_model.cpp ../../src/values_model.h)
add_test(NAME test_median COMMAND test_median)

target_link_libraries(test_median PRIVATE Qt${QT_VERSION_MAJOR}::Test)

