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

INSTALL(FILES
	interface/auto_bundle.h
	interface/auto_service.h
	interface/bundle.h
	interface/bundle_activator.h
	interface/bundle_context.h
	interface/bundle_event.h
	interface/bundle_exception.h
	interface/bundle_guid.h
	interface/bundle_listener.h
	interface/bundle_reference.h
	interface/class_not_found_exception.h
	interface/framework.h
	interface/framework_event.h
	interface/framework_exception.h
	interface/interrupted_exception.h
	interface/kshell.h
	interface/pragma.h
	interface/service.h
	interface/service_event.h
	interface/service_exception.h
	interface/service_listener.h
	interface/service_reference.h
	interface/service_wrapper.h
	interface/service_wrapper_impl.inl
DESTINATION include/kshell)
