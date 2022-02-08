#pragma once

#pragma comment ( lib, "d3d11.lib"  )
#pragma comment ( lib, "d3dx11.lib" )
#pragma comment ( lib, "d3dx10.lib" )

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

#include <cstdint>
#include <array>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <numbers>

//
// directx
//
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
