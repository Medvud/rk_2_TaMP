cmake_minimum_required(VERSION 3.16)
project(Project)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

add_executable(main ${CMAKE_SOURCE_DIR}/main.cpp)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  include_directories(
    ${CMAKE_SOURCE_DIR}/third-party/gtest/googletest/include
    ${CMAKE_SOURCE_DIR}/third-party/gtest/googlemock/include
  )

  add_executable(tests tests/test1.cpp)
  target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(tests gtest gtest_main gmock)
  add_test(test tests)
endif()

set(CPACK_PACKAGE_NAME "MyRK")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Medvud")
set(CPACK_PACKAGE_CONTACT "pishite@gmail.com")
set(CPACK_GENERATOR "DEB")

include(CPack)