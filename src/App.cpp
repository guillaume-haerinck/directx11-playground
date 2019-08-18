#include "pch.h"
#include "App.h"

#include "graphics/DXException.h"

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

App::App(HINSTANCE& hInstance) : m_className("hwd3dPlayground"), m_wc({ 0 }), m_hwnd(nullptr) {
	initWindow(hInstance);
	initDirectX11();
	initImGui();

	m_renderer = std::make_unique<Renderer>(m_dxo);
	m_rcommand = std::make_unique<RenderCommand>(m_dxo);
}

App::~App() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void App::Update(float dt) {
	m_rcommand->Clear();

	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Main debug window");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	m_renderer->BeginScene();

	m_rcommand->DrawTriangle();

	m_renderer->EndScene();

	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	m_rcommand->Swap();
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS /////////////////////////////
///////////////////////////////////////////////////////////////////////////

void App::initWindow(HINSTANCE& hInstance) {
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
}

void App::initDirectX11() {
	// Get viewport size
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	const int width = rc.right - rc.left;
	const int height = rc.bottom - rc.top;

	// SwapChain and Context description
	m_sd.BufferDesc.Width = width;
	m_sd.BufferDesc.Height = height;
	m_sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	m_sd.BufferDesc.RefreshRate.Numerator = 0;
	m_sd.BufferDesc.RefreshRate.Denominator = 0;
	m_sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	m_sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_sd.SampleDesc.Count = 1;
	m_sd.SampleDesc.Quality = 0;
	m_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_sd.BufferCount = 1; // Double buffering
	m_sd.OutputWindow = m_hwnd;
	m_sd.Windowed = TRUE;
	m_sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_sd.Flags = 0;

	// Create DirectX device
	DX::ThrowIfFailed(CALL_INFO,
		D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0u, nullptr, 0,
			D3D11_SDK_VERSION, &m_sd, &m_dxo.swapChain,
			&m_dxo.device, nullptr, &m_dxo.context
		)
	);

	// Get back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateRenderTargetView(backBuffer, nullptr, &m_dxo.target)
	);
	backBuffer->Release();

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_dxo.context->RSSetViewports(1, &vp);
}

void App::initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_dxo.device.Get(), m_dxo.context.Get());
	ImGui::StyleColorsDark();
}
