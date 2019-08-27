#include "pch.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "App.h"
#include "graphics/DXException.h"

struct test {
	float z;

	test(float z) : z(z) {}

	~test() {
		std::cout << "here";
	}
};

struct position {
	float x;
	float y;
	std::unique_ptr<test> myTest;
};

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	try {
		App* app = new App(hInstance);

		entt::registry registry;
		auto entity = registry.create();
		registry.assign<position>(entity, 1.f, 1.f, std::make_unique<test>(1.0f));

		registry.view<position>().each([](auto& pos) {
			pos.x += 5;
		});

		MSG msg = { 0 };
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// TODO handle game loop with deltatime and framerate
			app->Update(0.0f);
		}

		ID3D11Debug* debug = app->GetDebugDevice();
		delete app;

		if (debug != nullptr) {
			// All refCount must be 0, IntRef is not important
			debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			// delete debug;
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
