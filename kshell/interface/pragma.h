#ifndef PRAGMA_H
#define PRAGMA_H

#ifdef WIN32
#   pragma warning(disable : 4996)
#   pragma warning(disable : 4290)
#endif // WIN32

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN // avoid including unnecessary WIN32 header files
#	include <windows.h>
#	define BundleFunc extern "C" __declspec(dllexport) // WIN32 DLL exporter
#else
#	include <dlfcn.h>
#	define BundleFunc extern "C" // LINUX shared object exporter
#endif // WIN32

#endif // PRAGMA_H
