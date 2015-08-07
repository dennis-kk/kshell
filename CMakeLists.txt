# CMakeLists file
cmake_minimum_required(VERSION 2.6)

project (kshell)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/kshell
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

ADD_SUBDIRECTORY(kshell)
ADD_SUBDIRECTORY(bundle-sample)
