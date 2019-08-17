#include "pch.h"
#include "RenderCommand.h"

#include "DXException.h"

RenderCommand::RenderCommand(DXObjects dxObjects) : m_dxo(dxObjects)
{
	
}

void RenderCommand::Clear() const {
	const float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_dxo.context->ClearRenderTargetView(m_dxo.target.Get(), color);
}

void RenderCommand::Swap() const {
	m_dxo.swapChain->Present(1u, 0u);
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
		m_dxo.device->CreateBuffer(&bd, &sd, &vertexBuffer)
	);

	// Set vertex buffer to Input Assembler
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_dxo.context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Create pixel shader
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(L"legacyPS.cso", &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreatePixelShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &pixelShader
		)
	);

	// Bind pixel shader
	m_dxo.context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	// Create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(L"legacyVS.cso", &blob)
	);
	DX::ThrowIfFailed(CALL_INFO, 
		m_dxo.device->CreateVertexShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &vertexShader
		)
	);

	// Bind vertex shader
	m_dxo.context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	// Create input buffer layout
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateInputLayout(
			ied, (UINT)std::size(ied),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout
		)
	);

	// Bind input buffer layout
	m_dxo.context->IASetInputLayout(inputLayout.Get());

	// Bind render target
	m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), nullptr);

	// Set primitive topology
	m_dxo.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Issue the draw call
	m_dxo.context->Draw(std::size(vertices), 0u);
}
