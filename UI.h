#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "KeiFont.cpp"

class UI {
public:
	HWND CreateWindowApplication(LPCWSTR windowName, int width, int height);
	bool InitializeInterface(HWND hWindow);
	void NewFrame();
	void RenderFrame();

	static bool dxSwapChainOccluded;
	static ID3D11Device* dxDevice;
	static ID3D11DeviceContext* dxDeviceContext;
	static IDXGISwapChain* dxSwapChain;
	static ID3D11RenderTargetView* dxRenderTarget;

private:
	HWND hWindow;

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();

	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};