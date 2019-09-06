#include "pch.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "App.h"
#include "graphics/DXException.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	try {
		App* app = new App(hInstance);

		MSG msg = { 0 };
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// TODO handle game loop with deltatime and framerate
			app->Update(0.0f);
		}

		auto debug = app->GetDebugDevice();
		delete app;

		if (debug.get() != nullptr) {
			// All refCount must be 0, IntRef is not important
			debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}

		_CrtDumpMemoryLeaks();
		return static_cast<int>(msg.wParam);

	} catch (const DXException& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	} catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
	} catch (...) {
		MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}