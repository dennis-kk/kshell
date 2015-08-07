# CMakeLists file
cmake_minimum_required(VERSION 2.6)

SET(CMAKE_CXX_FLAGS "-g -O2 -Wall -std=c++0x")

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/kshell/interface
  ${PROJECT_SOURCE_DIR}/bundle-sample/my_bundle
)

add_library (my_bundle SHARED
	./my_bundle/export.cpp
	./my_bundle/my_bundle_activator.cpp
)

add_library (caller_bundle SHARED
	./caller_bundle/export.cpp
	./caller_bundle/caller_bundle_activator.cpp
)
