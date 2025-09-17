cmake_minimum_required(VERSION 3.16)
project(Project)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(CPACK_PACKAGE_NAME "MyRK")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Medvud")
set(CPACK_PACKAGE_CONTACT "pishite@gmail.com")
set(CPACK_GENERATOR "DEB")
include(CPack)