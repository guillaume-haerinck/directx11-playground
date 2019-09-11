#include "pch.h"
#include "App.h"

#include "graphics/DXException.h"
#include "graphics/ConstantBuffer.h"

#include "components/singletons/graphics/Samplers.h"
#include "components/singletons/graphics/ConstantBuffers.h"
#include "components/singletons/graphics/Materials.h"
#include "components/singletons/graphics/Camera.h"
#include "components/singletons/graphics/Lights.h"
#include "components/singletons/io/Inputs.h"

#include "examples/basics/basic-triangle/BasicTriangle.h"
#include "examples/basics/rotating-cube/RotatingCube.h"
#include "examples/basics/textured-primitives/TexturedPrimitives.h"
#include "examples/basics/model-loading/ModelLoading.h"

#include "examples/blinn-phong/point-light/PointLight.h"
#include "examples/blinn-phong/multiple-light-types/MultipleLightTypes.h"
#include "examples/blinn-phong/materials/Materials.h"

bool App::isContexInit = false;

App::App(HINSTANCE& hInstance) : m_className("hwd3dPlayground"), m_hwnd(nullptr) {
	initWindow(hInstance);
	initDirectX11();
	initImGui();
	m_ctx.rcommand = std::make_unique<RenderCommand>(m_dxo);
	resetAppTo<phongExample::MultipleLightTypes>();
}

App::~App() {
	m_dxo.context->ClearState();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	App* me = (App*) (GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (me && App::isContexInit && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		return me->memberWndProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT App::memberWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto ioEntity = m_ctx.singletonComponents.at(SingletonComponents::IO);
	scomp::Inputs& inputs = m_ctx.registry.get<scomp::Inputs>(ioEntity);
	auto graphicEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
	scomp::Camera& camera = m_ctx.registry.get<scomp::Camera>(graphicEntity);

	switch (msg) {
	case WM_LBUTTONDBLCLK:
		inputs.actionState.at(scomp::InputAction::CAM_RESET) = true;
		break;

	case WM_MOUSEWHEEL:
		inputs.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		inputs.actionState.at(scomp::InputAction::CAM_DOLLY) = true;
		break;

	case WM_MOUSEMOVE: {
		int newPosX = GET_X_LPARAM(lParam);
		int newPosY = GET_Y_LPARAM(lParam);
		inputs.delta.x = inputs.mousePos.x - newPosX;
		inputs.delta.y = inputs.mousePos.y - newPosY;
		inputs.mousePos.x = newPosX;
		inputs.mousePos.y = newPosY;

		if (wParam == MK_LBUTTON) { inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = true; }
		if (wParam == MK_MBUTTON) { inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = true; }
		if (wParam == MK_RBUTTON) { inputs.actionState.at(scomp::InputAction::CAM_PAN) = true; }

		break;
	}

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void App::Update(float dt) {
	m_ctx.rcommand->Clear();

	// Update GUI
	{
		// Start new frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Update each window
		m_activeExemple->ImGuiUpdate();
		renderMenu();
		//ImGui::ShowDemoWindow();

		// End frame
		ImGui::EndFrame();
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

	// Reset input states (nescessary as WndProc is not called every frame)
	{
		auto ioEntity = m_ctx.singletonComponents.at(SingletonComponents::IO);
		scomp::Inputs& inputs = m_ctx.registry.get<scomp::Inputs>(ioEntity);
		inputs.actionState.fill(false);
	}

	m_ctx.rcommand->Swap();
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS /////////////////////////////
///////////////////////////////////////////////////////////////////////////

void App::renderMenu() {
	ImGui::Begin("Main debug window");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::CollapsingHeader("Help")) {
		ImGui::Text("Camera controls :");
		ImGui::BulletText("Orbit - Left mouse button / Middle mouse button");
		ImGui::BulletText("Pan - Right mouse button");
		ImGui::BulletText("Zoom - Mousewheel");
		ImGui::BulletText("Reset - Left mouse double click");
	}

	ImGui::Spacing();

	ImGui::Text("Exemples:");
	if (ImGui::CollapsingHeader("Basic")) {
		if (ImGui::Button("Basic triangle")) { resetAppTo<basicExample::BasicTriangle>(); }
		if (ImGui::Button("Rotating cube")) { resetAppTo<basicExample::RotatingCube>(); }
		if (ImGui::Button("Textured primitives")) { resetAppTo<basicExample::TexturedPrimitives>(); }
		if (ImGui::Button("Model loading")) { resetAppTo<basicExample::ModelLoading>(); }
	}

	// if (ImGui::CollapsingHeader("Intermediate")) {}

	// if (ImGui::CollapsingHeader("Advanced")) {}

	if (ImGui::CollapsingHeader("Blinn Phong shading")) {
		if (ImGui::Button("Point light")) { resetAppTo<phongExample::PointLight>(); }
		if (ImGui::Button("Multiple light types")) { resetAppTo<phongExample::MultipleLightTypes>(); }
		if (ImGui::Button("Materials")) { resetAppTo<phongExample::Materials>(); }
	}

	// if (ImGui::CollapsingHeader("Toon shading")) {}

	// if (ImGui::CollapsingHeader("PBR shading")) {}

	ImGui::End();
}

void App::initWindow(HINSTANCE& hInstance) {
	// Register window class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_DBLCLKS;
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

	// First step to access windows callback as a member function
	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR) this);

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

	// Init non-optional constant buffers
	scomp::ConstantBuffers cbs = {};
	comp::ConstantBuffer perFrameCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::perFrame));
	comp::ConstantBuffer perMeshCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::perMesh));
	cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME) = perFrameCB;
	cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH) = perMeshCB;
	m_ctx.registry.assign<scomp::ConstantBuffers>(entity, cbs);

	// Init materials
	scomp::PhongMaterials phongMaterials = {};
	m_ctx.registry.assign<scomp::PhongMaterials>(entity, phongMaterials);
	scomp::CookTorranceMaterials cookMaterials = {};
	m_ctx.registry.assign<scomp::CookTorranceMaterials>(entity, cookMaterials);

	// Init camera
	scomp::Camera camera = {};
	camera.position = XMFLOAT3(0.0f, 0.0f, 6.0f);
	camera.target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);
	XMMATRIX view = XMMatrixTranslation(camera.position.x, camera.position.y, camera.position.z);
	XMStoreFloat4x4(&camera.proj, proj);
	XMStoreFloat4x4(&camera.view, view);
	m_ctx.registry.assign<scomp::Camera>(entity, camera);

	// Init lights
	scomp::Lights lights = {};
	m_ctx.registry.assign<scomp::Lights>(entity, lights);

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

void App::initIOSingletonEntity() {
	auto entity = m_ctx.registry.create();
	m_ctx.singletonComponents.at(SingletonComponents::IO) = entity;

	// Init inputs
	scomp::Inputs inputs = {};
	m_ctx.registry.assign<scomp::Inputs>(entity, inputs);
}
