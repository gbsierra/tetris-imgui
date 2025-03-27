#include "UI.h"

// Define Static Variables
ID3D11Device* UI::dxDevice = NULL;
ID3D11DeviceContext* UI::dxDeviceContext = NULL;
IDXGISwapChain* UI::dxSwapChain = nullptr;
ID3D11RenderTargetView* UI::dxRenderTarget = nullptr;
bool UI::dxSwapChainOccluded = false;

// Create Window
HWND UI::CreateWindowApplication(LPCWSTR windowName, int width, int height) {
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, windowName, NULL };
	::RegisterClassEx(&wc);
	hWindow = ::CreateWindow(wc.lpszClassName, windowName, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2), width, height, NULL, NULL, wc.hInstance, NULL);
	::ShowWindow(hWindow, SW_SHOWDEFAULT);
	::UpdateWindow(hWindow);
	return hWindow;
}

// Initialize ImGui Interface
bool UI::InitializeInterface(HWND hWindow) {
    if (!UI::CreateDeviceD3D(hWindow)) {
        CleanupDeviceD3D();
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = NULL;

    ImFontConfig font_cfg = ImFontConfig();
    font_cfg.OversampleH = font_cfg.OversampleV = 0;
    font_cfg.PixelSnapH = true;
	font_cfg.RasterizerDensity = 1.25f;
    font_cfg.SizePixels = 14.0f;
    ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(Kei_compressed_data, Kei_compressed_size, font_cfg.SizePixels, &font_cfg);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().Alpha = 1.f;

	::ImGui_ImplWin32_Init(hWindow);
	::ImGui_ImplDX11_Init(dxDevice, dxDeviceContext);
	return true;
}

void UI::NewFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void UI::RenderFrame() {
    static ImVec4 Clear = ImVec4(0.f, 0.f, 0.f, 0.f);
    ImGui::EndFrame();
    ImGui::Render();
    dxDeviceContext->OMSetRenderTargets(1, &dxRenderTarget, NULL);
    dxDeviceContext->ClearRenderTargetView(dxRenderTarget, (float*)&Clear);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    HRESULT swapChainResult = dxSwapChain->Present(0, 0);
    dxSwapChainOccluded = (swapChainResult == DXGI_STATUS_OCCLUDED);
}

// Initialize Direct3D
bool UI::CreateDeviceD3D(HWND hWnd) {
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &dxSwapChain, &dxDevice, &featureLevel, &dxDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &dxSwapChain, &dxDevice, &featureLevel, &dxDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UI::CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (dxSwapChain) { dxSwapChain->Release(); dxSwapChain = nullptr; }
    if (dxDeviceContext) { dxDeviceContext->Release(); dxDeviceContext = nullptr; }
    if (dxDevice) { dxDevice->Release(); dxDevice = nullptr; }
}

void UI::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    dxSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    dxDevice->CreateRenderTargetView(pBackBuffer, nullptr, &dxRenderTarget);
    pBackBuffer->Release();
}

void UI::CleanupRenderTarget() {
    if (dxRenderTarget) { dxRenderTarget->Release(); dxRenderTarget = nullptr; }
}

// WndProc
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (dxDevice != NULL && wParam != SIZE_MINIMIZED) {
			CleanupRenderTarget();
			dxSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}