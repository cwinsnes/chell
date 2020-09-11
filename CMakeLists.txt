cmake_minimum_required(VERSION 2.8)

project(chell)

# includes
include_directories(
  include
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


# find header & source
file(GLOB_RECURSE HEADER "include/*.h")
file(GLOB_RECURSE SOURCE "src/*.c")

add_executable(${PROJECT_NAME}
  ${SOURCE}
  ${HEADER}
)

source_group("Header include" FILES ${HEADER})
source_group("Source src"     FILES ${SOURCE})
