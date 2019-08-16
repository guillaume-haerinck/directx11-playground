#include "pch.h"
#include "RenderCommand.h"

#include "DXException.h"

RenderCommand::RenderCommand(HWND& hWnd)
	: m_sd({ 0 })
{
	// Get viewport size
	RECT rc;
	GetClientRect(hWnd, &rc);
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
	m_sd.OutputWindow = hWnd;
	m_sd.Windowed = TRUE;
	m_sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_sd.Flags = 0;

	// Create DirectX device
	DX::ThrowIfFailed(CALL_INFO,
		D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_DEBUG, nullptr, 0,
			D3D11_SDK_VERSION, &m_sd, &m_swap,
			&m_device, nullptr, &m_context
		)
	);

	// Get back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	DX::ThrowIfFailed(CALL_INFO,
		m_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_device->CreateRenderTargetView(backBuffer, nullptr, &m_target)
	);
	backBuffer->Release();

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float) width;
	vp.Height = (float) height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_context->RSSetViewports(1, &vp);
}

void RenderCommand::Clear() const {
	const float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_target.Get(), color);
}

void RenderCommand::Swap() const {
	m_swap->Present(1u, 0u);
}

void RenderCommand::DrawTriangle() {
	struct Vertex {
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{  0.0f,  0.5f },
		{  0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	// Input buffer layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	DX::ThrowIfFailed(CALL_INFO,
		m_device->CreateBuffer(&bd, &sd, &vertexBuffer)
	);

	// Set vertex buffer to Input Assembler
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Create pixel shader
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(L"legacyPS.cso", &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_device->CreatePixelShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &pixelShader
		)
	);

	// Bind pixel shader
	m_context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	// Create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(L"legacyVS.cso", &blob)
	);
	DX::ThrowIfFailed(CALL_INFO, 
		m_device->CreateVertexShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &vertexShader
		)
	);

	// Bind vertex shader
	m_context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	// Create input buffer layout
	DX::ThrowIfFailed(CALL_INFO,
		m_device->CreateInputLayout(
			ied, (UINT)std::size(ied),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout
		)
	);

	// Bind input buffer layout
	m_context->IASetInputLayout(inputLayout.Get());

	// Bind render target
	m_context->OMSetRenderTargets(1u, m_target.GetAddressOf(), nullptr);

	// Set primitive topology
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Issue the draw call
	m_context->Draw(std::size(vertices), 0u);
}
