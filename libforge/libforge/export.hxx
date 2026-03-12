#pragma once

#if defined(LIBFORGE_STATIC)
#  define LIBFORGE_SYMEXPORT
#elif defined(LIBFORGE_STATIC_BUILD)
#  define LIBFORGE_SYMEXPORT
#elif defined(LIBFORGE_SHARED)
#  ifdef _WIN32
#    define LIBFORGE_SYMEXPORT __declspec (dllimport)
#  else
#    define LIBFORGE_SYMEXPORT
#  endif
#elif defined(LIBFORGE_SHARED_BUILD)
#  ifdef _WIN32
#    define LIBFORGE_SYMEXPORT __declspec (dllexport)
#  else
#    define LIBFORGE_SYMEXPORT
#  endif
#else
// If none of the above macros are defined, then we assume we are being used by
// some third-party build system that cannot/doesn't signal the library type
// buing linked.
//
#  define LIBFORGE_SYMEXPORT
#endif
