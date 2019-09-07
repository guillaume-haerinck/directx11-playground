#include "pch.h"
#include "App.h"

#include <dxgidebug.h>

#include "graphics/DXException.h"

#include "examples/basics/basic-triangle/BasicTriangle.h"
#include "examples/basics/rotating-cube/RotatingCube.h"
#include "examples/basics/textured-primitives/TexturedPrimitives.h"
#include "examples/basics/model-loading/ModelLoading.h"

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

App::App(HINSTANCE& hInstance) : m_className("hwd3dPlayground"), m_hwnd(nullptr) {
	initWindow(hInstance);
	initDirectX11();
	initImGui();

	m_ctx.rcommand = std::make_unique<RenderCommand>(m_dxo);
	m_activeExemple = std::make_unique<exemple::TexturedPrimitives>(m_ctx);

	initGraphicSingletonEntity();
}

App::~App() {
	m_dxo.context->ClearState();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void App::Update(float dt) {
	m_ctx.rcommand->Clear();

	// Update GUI
	{
		m_activeExemple->ImGuiUpdate();
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Main debug window");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}
	
	// Update Geometry
	{
		m_activeExemple->Update();
	}
	
	// Render
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	m_ctx.rcommand->Swap();
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS /////////////////////////////
///////////////////////////////////////////////////////////////////////////

void App::initWindow(HINSTANCE& hInstance) {
	// Register window class
	WNDCLASSEX wc;
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
	wc.lpszClassName = m_className;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

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
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1; // Double buffering
	sd.OutputWindow = m_hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT createDeviceFlags = 0;
#ifndef NDEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Create DirectX device
	DX::ThrowIfFailed(CALL_INFO,
		D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_dxo.swapChain,
			&m_dxo.device, nullptr, &m_dxo.context
		)
	);

#ifndef NDEBUG
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debugDevice))
	);
#endif

	// Get back buffer
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_dxo.target)
	);

	// Create the depth stencil state
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateDepthStencilState(&dsDesc, &depthStencilState)
	);

	// Bind depth stencil state
	m_dxo.context->OMSetDepthStencilState(depthStencilState.Get(), 1);

	// Create depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateTexture2D(&descDepth, nullptr, &depthStencil)
	);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateDepthStencilView(depthStencil.Get(), &descDSV, &m_dxo.depthStencilView)
	);

	// Change default rasterizer to have Counter Clock-Wise Winding order (to support gltf models)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = true;
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = false;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateRasterizerState(&rd, &rasterizer)
	);
	m_dxo.context->RSSetState(rasterizer.Get());

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_dxo.context->RSSetViewports(1, &vp);

	// Bind what will not change for a while
	m_dxo.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), m_dxo.depthStencilView.Get());
}

void App::initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_dxo.device.Get(), m_dxo.context.Get());
	ImGui::StyleColorsDark();
}

void App::initGraphicSingletonEntity() {
	auto entity = m_ctx.registry.create();
	m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC) = entity;

	// Init texture samplers
	scomp::Sampler sampler0 = m_ctx.rcommand->CreateSampler(scomp::SamplerSlot::ANISOTROPIC_WRAP);
	scomp::Sampler sampler1 = m_ctx.rcommand->CreateSampler(scomp::SamplerSlot::LINEAR_CLAMP);
	scomp::Samplers samplers = {};
	samplers.samplers = { sampler0, sampler1 };
	m_ctx.registry.assign<scomp::Samplers>(entity, samplers);

	// Bind texture samplers
	m_ctx.rcommand->BindSampler(sampler0);
	m_ctx.rcommand->BindSampler(sampler1);
}
