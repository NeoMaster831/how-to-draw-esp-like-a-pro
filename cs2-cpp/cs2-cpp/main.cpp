#include "Esper.h"
#include "GDI.h"

LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam)) return 0;
	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	switch (message) {
	case WM_NCHITTEST: {
		const LONG borderWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		const LONG titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
		POINT cursorPos = { GET_X_LPARAM(wParam), GET_Y_LPARAM(lParam) };
		RECT windowRect;
		GetWindowRect(window, &windowRect);

		if (cursorPos.y >= windowRect.top && cursorPos.y < windowRect.top * titleBarHeight) return HTCAPTION;

		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(window, message, wParam, lParam);
}

INT APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT cmd_show) {

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = windowProcedure;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Esper0";
	wc.lpszMenuName = L"Esper1";

	RegisterClassExW(&wc);
	
	HWND hwnd = CreateWindowExW(
		0, L"Esper2", L"Esper3",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hwnd, cmd_show);
	UpdateWindow(hwnd);

	Esper esper(L"cs2.exe", L"client.dll", L"Counter-Strike 2", wc);
	
	MSG msg;
	while (GetMessage(&msg, hwnd, NULL, NULL)) {

	}
	
}