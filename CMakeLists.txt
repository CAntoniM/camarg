cmake_minimum_required(VERSION 3.20)

project(camarg)

enable_testing()

file(GLOB camarg_src "src/*.cpp")

add_library(camarg SHARED ${camarg_src})
target_include_directories(camarg PUBLIC "inc")

add_subdirectory(test) 

find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)

file(GLOB dox_src "inc/*.h")

doxygen_add_docs(
    docgen
    ${dox_src}
    COMMENT "Generate man pages"
)

add_dependencies(camarg docgen)