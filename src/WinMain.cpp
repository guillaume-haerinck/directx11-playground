#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	const auto pClassName = "hwd3dPlayground";

	// Register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// Create window instance
	HWND hWnd = CreateWindowEx(
		0, pClassName, "DirectX 11 Playground",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480, nullptr, nullptr,
		hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	switch (gResult) {
		case -1: return -1;
		default: return msg.wParam;
	}
}
