#pragma once

// Windows API
#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Common
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <sstream>

// Data structures
#include <vector>
#include <string>
#include <stdint.h>

// DirectX
#include <D3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dxgidebug.h>
#include <wrl.h>
#include <d3dcompiler.h>

// Libs
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
