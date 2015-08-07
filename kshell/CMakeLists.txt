# CMakeLists file
cmake_minimum_required(VERSION 2.6)

project (kshell)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/interface/
  ${PROJECT_SOURCE_DIR}/internal/
  ${PROJECT_SOURCE_DIR}/lua/
)

SET(CMAKE_CXX_FLAGS "-g -O2 -Wall -std=c++0x")

add_executable(kshell
	internal/bundle_box.cpp
	internal/bundle_box_loader.cpp
	internal/bundle_context_impl.cpp
	internal/bundle_impl.cpp
	internal/bundle_loader.cpp
	internal/bundle_manager.cpp
	internal/bundle_reference_impl.cpp
	internal/condition.cpp
	internal/daemon_linux.cpp
	internal/framework_factory.cpp
	internal/framework_impl.cpp
	internal/misc.cpp
	internal/path.cpp
	internal/pid_file.cpp
	internal/resolver.cpp
	internal/scope_lock.cpp
	internal/service_reference_impl
	internal/util.cpp
	lua/lapi.c
	lua/lauxlib.c
	lua/lbaselib.c
	lua/lbitlib.c
	lua/lcode.c
	lua/lcorolib.c
	lua/lctype.c
	lua/ldblib.c
	lua/ldebug.c
	lua/ldo.c
	lua/ldump.c
	lua/lfunc.c
	lua/lgc.c
	lua/linit.c
	lua/liolib.c
	lua/llex.c
	lua/lmathlib.c
	lua/lmem.c
	lua/loadlib.c
	lua/lobject.c
	lua/lopcodes.c
	lua/loslib.c
	lua/lparser.c
	lua/lstate.c
	lua/lstring.c
	lua/lstrlib.c
	lua/ltable.c
	lua/ltablib.c
	lua/ltm.c
	lua/lundump.c
	lua/lvm.c
	lua/lzio.c
)

target_link_libraries(kshell -lpthread -ldl)
