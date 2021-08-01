#pragma once

//
// macros
//

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define _HAS_EXCEPTIONS 0
#define _ITERATOR_DEBUG_LEVEL 0
#define _CRT_SECURE_INVALID_PARAMETER
#define _CRT_SECURE_NO_WARNINGS

#define NOMINMAX

#define NOINLINE __declspec( noinline )
#define EXPORT __declspec( dllexport )

//
// types
//
using ulong_t = unsigned long;

//
// windows and stl
//
#include <Windows.h>

//
// dependencies
//


//
// globals
//


//
// misc
//

