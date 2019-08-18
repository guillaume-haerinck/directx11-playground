#include "pch.h"
#include "basic-triangle.h"

#include "graphics/DXException.h"


namespace exemple {
	BasicTriangle::BasicTriangle(DXObjects dxObjects)
		: m_dxo(dxObjects) {
		m_shader = std::make_unique<Shader>(m_dxo, L"legacyVS.cso", L"legacyPS.cso");

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
		const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Create vertex buffer
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
			m_dxo.device->CreateBuffer(&bd, &sd, &m_vertexBuffer)
		);

		// Set vertex buffer to Input Assembler
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		// Create input buffer layout
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateInputLayout(
				ied, (UINT)std::size(ied),
				m_shader->GetVertexShaderBlob()->GetBufferPointer(),
				m_shader->GetVertexShaderBlob()->GetBufferSize(),
				&m_inputLayout
			)
		);
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		// Bind input buffer layout
		m_dxo.context->IASetInputLayout(m_inputLayout.Get());

		// Bind render target
		m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), nullptr);

		// Set primitive topology
		m_dxo.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_shader->Bind();

		// Draw the triangle
		m_dxo.context->Draw(3u, 0u);
	}

	void BasicTriangle::ImGuiUpdate() {
	}
}
