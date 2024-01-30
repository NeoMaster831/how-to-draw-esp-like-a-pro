#pragma once
#include "Defines.h"
#include <windowsx.h>
#include <dwmapi.h>
#include <d3d11.h>
#include "./ImGui/imgui.h"
#include "./ImGui/imgui_impl_dx11.h"
#include "./ImGui/imgui_impl_win32.h"

class Esper;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct _tParam {
	HWND* window;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	D3D_FEATURE_LEVEL level;
	Esper* esperClass;
} tParam, * ptParam;

typedef struct _RGB {
	ImU32 R, G, B;
} RGB;

class GDI
{
	// GDI - Render with ImGUI

	// Set FPS to you want, my computer is 240hz so.
	unsigned int fps = 240;

	Defines* defp;
	Esper* super;
	HWND window;
	int screenX, screenY;
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* deviceContext{ nullptr };
	IDXGISwapChain* swapChain{ nullptr };
	ID3D11RenderTargetView* renderTargetView{ nullptr };
	D3D_FEATURE_LEVEL level{};

public:

	
	GDI(LPCWSTR window, Defines* dp, WNDCLASSEX& wndClassEx, Esper* super);

	static ImU32 _Color(RGB color) {
		return IM_COL32(color.R, color.G, color.B, 255);
	}

	static void DrawRect(int x, int y, int w, int h, RGB color, int thickness) {
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2((float)x, (float)y), 
			ImVec2((float)(x + w), (float)(y + h)), _Color(color), 0, 0, (float)thickness);
	}

	static pair<int, int> CalculateWindow(int x, int y, HWND window) {
		RECT rect;
		if (!GetWindowRect(window, &rect)) throw 0x3d00000d;
		return { rect.left + x, rect.top + y };
	}
};