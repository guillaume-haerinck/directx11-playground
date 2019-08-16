#include "pch.h"
#include "App.h"

#include "graphics/DXException.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

App::App(HINSTANCE& hInstance)
	: m_className("hwd3dPlayground"), m_wc({ 0 }), m_hwnd(nullptr),
	  m_rc(nullptr), m_renderer(nullptr)
{
	// Register window class
	m_wc.cbSize = sizeof(m_wc);
	m_wc.style = CS_OWNDC;
	m_wc.lpfnWndProc = WndProc;
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = hInstance;
	m_wc.hIcon = nullptr;
	m_wc.hCursor = nullptr;
	m_wc.hbrBackground = nullptr;
	m_wc.lpszMenuName = nullptr;
	m_wc.lpszClassName = m_className;
	m_wc.hIconSm = nullptr;
	RegisterClassEx(&m_wc);

	// Create window instance
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hwnd = CreateWindowEx(
		0, m_className, "DirectX 11 Playground",
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance, nullptr
	);
	if (m_hwnd == nullptr) {
		throw DX_LAST_ERROR_EXCEPTION;
	}

	ShowWindow(m_hwnd, SW_SHOW);

	m_rc = new RenderCommand(m_hwnd);
	m_renderer = new Renderer(*m_rc);
}

App::~App() {
	delete m_rc;
}

void App::Update(float dt) {
	m_rc->Clear();

	m_renderer->BeginScene();

	m_rc->DrawTriangle();

	m_renderer->EndScene();

	m_rc->Swap();
}

