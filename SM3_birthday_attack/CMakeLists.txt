cmake_minimum_required(VERSION 3.0.0)
project(SM3_birthday_attack VERSION 0.1.0)


find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIRS})
  message(STATUS "OpenSSL Found!")
endif()
set(CMAKE_BUILD_TYPE "RELEASE")
add_executable(SM3_birthday_attack attack.cpp sm3.h )
target_link_libraries(SM3_birthday_attack OpenSSL::Crypto OpenSSL::SSL)
