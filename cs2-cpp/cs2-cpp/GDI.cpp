#include "GDI.h"
#include "Esper.h"

void drawEsper(Esper* esperClass, HWND hwnd) {

	auto pawns = esperClass->getEntityPawns();
	auto resolution = esperClass->def.getResolution();
	auto viewMat = esperClass->def.getViewMatrix();

	for (int i = 0; i < pawns.size(); i++) {
		auto location = esperClass->def.getEntityLocation(pawns[i]);
		Vector3 scPos;
		try {
			scPos = esperClass->worldToScreen(location, viewMat, resolution);
		}
		catch (int expn) {
			if (expn == 0x3d000007) continue;
			else ExitProcess(0x3d000008);
		}

		auto headLoc = Vector3{ location.x, location.y, location.z + 75.f };
		Vector3 headPos;
		try {
			headPos = esperClass->worldToScreen(headLoc, viewMat, resolution);
		}
		catch (int expn) {
			if (expn == 0x3d000007) continue;
			else ExitProcess(0x3d000008);
		};
		auto newHeadPos = GDI::CalculateWindow((int)headPos.x, (int)headPos.y, hwnd);
		auto newScPos = GDI::CalculateWindow((int)scPos.x, (int)scPos.y, hwnd);

		float height = (float)(newScPos.second - newHeadPos.second);
		float width = height * 2.4f;

		GDI::DrawRect((int)(newHeadPos.first - width / 16.f), 
			(int)(newHeadPos.second + 30.f),
			(int)(width / 4.f), (int)(height * 1.2f), RGB{ 255, 0, 0 }, 3);
	}
}

DWORD WINAPI renderThread(ptParam param) {

	auto window = param->window;
	auto deviceContext = param->deviceContext;
	auto renderTargetView = param->renderTargetView;
	auto swapChain = param->swapChain;
	auto device = param->device;
	auto esperClass = param->esperClass;

	bool forceQuit = false;

	while (!forceQuit) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) forceQuit = true;
		}
		if (forceQuit) break;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Render start
		if (esperClass->running) {
			drawEsper(esperClass, *window);
		}
		// Render end

		ImGui::Render();
		float color[4] = { 0, 0, 0, 0 };
		deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
		deviceContext->ClearRenderTargetView(renderTargetView, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swapChain->Present(0, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (swapChain) swapChain->Release();
	if (deviceContext) deviceContext->Release();
	if (device) device->Release();
	renderTargetView->Release();

	return 0;
}

GDI::GDI(LPCWSTR window, Defines* dp, WNDCLASSEX& wndClassEx, Esper* super) {

	this->window = FindWindow(NULL, window);
	if (this->window == NULL) throw 0x3d00000c;

	this->super = super;
	this->defp = dp;
	auto resolution = dp->getResolution();
	this->screenX = resolution.first, this->screenY = resolution.second;

	const HWND overlay = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wndClassEx.lpszClassName,
		wndClassEx.lpszMenuName,
		WS_POPUP,
		0,
		0,
		this->screenX,
		this->screenY,
		nullptr, nullptr,
		wndClassEx.hInstance,
		nullptr		
	);

	SetLayeredWindowAttributes(overlay, RGB(0, 0, 0), 255, LWA_ALPHA);

	RECT clientArea{};
	GetClientRect(overlay, &clientArea);

	RECT windowArea{};
	GetWindowRect(overlay, &windowArea);

	POINT diff{};
	ClientToScreen(overlay, &diff);

	const MARGINS margins{
		windowArea.left + (diff.x - windowArea.left),
		windowArea.top + (diff.y - windowArea.top),
		clientArea.right,
		clientArea.bottom,
	};

	DwmExtendFrameIntoClientArea(overlay, &margins);

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = this->fps;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = overlay;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0, levels, 2,
		D3D11_SDK_VERSION,
		&sd,
		&swapChain,
		&device,
		&level,
		&deviceContext
	);

	ID3D11Texture2D* backBuffer{ nullptr };
	swapChain->GetBuffer(0U, IID_PPV_ARGS(&backBuffer));

	if (backBuffer) {
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();
	}
	else throw 0x3d000009;
	if (renderTargetView == 0) throw 0x3d00000a;

	ShowWindow(overlay, SW_SHOW);
	UpdateWindow(overlay);

	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGui_ImplWin32_Init(overlay);
	ImGui_ImplDX11_Init(device, deviceContext);

	auto param = tParam{
		&this->window, device, deviceContext, swapChain, renderTargetView, level, this->super
	};

	auto handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)renderThread, &param, NULL, NULL);
	if (handle != NULL) CloseHandle(handle);
	else throw 0x3d00000b;
}