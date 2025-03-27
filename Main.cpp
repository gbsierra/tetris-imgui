#include <chrono>

#include "UI.h"
#include "Game.h"

UI UserInterface = UI();
Game Tetris = Game();

using namespace std::chrono;

int main() {
	// Create a Window
	HWND WindowApplication = UserInterface.CreateWindowApplication(L"Tetris", 625, 800);
	if (WindowApplication == NULL) {
		std::cout << "Failed to create window application." << std::endl;
		return 1;
	}

	// Initialize ImGui
	if (!UserInterface.InitializeInterface(WindowApplication))
		return 1;

	// Initialize The Game
	Tetris.Initialize();

	// Main Loop
	bool IsFinished = false;
	while (!IsFinished) {
		// Handle Polling Events
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				IsFinished = true;
		}

		// Handle Window Being Minimized Or Screen Locked
		if (UserInterface.dxSwapChainOccluded && UserInterface.dxSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
			::Sleep(10);
			continue;
		} UserInterface.dxSwapChainOccluded = false;

		// Handle Game Logic
		if (!Tetris.Update())
			IsFinished = true;

		// Actual Rendering
		UserInterface.NewFrame();

		Tetris.Render();

		UserInterface.RenderFrame();
	}

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	::DestroyWindow(WindowApplication);
	return 0;
}