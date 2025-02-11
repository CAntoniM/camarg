cmake_minimum_required(VERSION 3.20)

project(camarg)

enable_testing()

file(GLOB camarg_src "src/*.cpp")

add_library(camarg SHARED ${camarg_src})
target_include_directories(camarg PUBLIC "inc")

add_subdirectory(test) 
