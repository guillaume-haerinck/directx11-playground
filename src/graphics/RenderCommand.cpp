#include "pch.h"
#include "RenderCommand.h"

#include "DXException.h"

RenderCommand::RenderCommand(HWND& hWnd)
	: m_sd({ 0 })
{
	// SwapChain and Context description
	m_sd.BufferDesc.Width = 0;
	m_sd.BufferDesc.Height = 0;
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
	/*
	ID3D11Resource* backBuffer = nullptr;
	m_swap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer));
	m_device->CreateRenderTargetView(backBuffer, nullptr, &m_target);
	backBuffer->Release();
	*/
}

void RenderCommand::Clear() {
	//const float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//m_context->ClearRenderTargetView(m_target.Get(), color);
}

void RenderCommand::Swap() {
	m_swap->Present(1u, 0u);
}
