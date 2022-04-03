cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(request-handler)

add_library(request STATIC
  request.h
  request.cpp
)

add_executable(request_app request_app.cpp)

target_link_libraries(request_app request)
