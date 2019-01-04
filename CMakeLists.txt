cmake_minimum_required(VERSION 3.1.0)

project(qaria2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
## Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/forms")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
  src/main.cpp
  src/mainwindow.cpp
  src/core.cpp
)

set(HEADERS
  include/mainwindow.hpp
  include/core.hpp
)

set(UI
  forms/mainwindow.ui
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI})
qt5_use_modules(${PROJECT_NAME} Widgets)
