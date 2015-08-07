#ifndef BUNDLE_HANDLE_H
#define BUNDLE_HANDLE_H

#include "pragma.h"

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN // avoid including unnecessary WIN32 header files
#	include <windows.h>
#	define OsHandle HMODULE // DLL handle
#else
#	define OsHandle void* // LINUX SO handle
#endif // WIN32

typedef void* (*NewActivatorFunc)();
typedef void* (*NewServiceFunc)(const char*);
typedef void* (*DestroyServiceFunc)(void*, const char*);

#endif // BUNDLE_HANDLE_H
