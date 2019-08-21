#include "pch.h"
#include "basic-triangle.h"

#include "graphics/DXException.h"


namespace exemple {
	BasicTriangle::BasicTriangle(DXObjects dxObjects) : m_dxo(dxObjects) {
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_shader = std::make_unique<Shader>(m_dxo, ied, ARRAYSIZE(ied), L"noRessourceVS.cso", L"noRessourcePS.cso");

		struct Vertex {
			float x;
			float y;
		};

		const Vertex vertices[] = {
			{  0.0f,  0.5f },
			{  0.5f, -0.5f },
			{ -0.5f, -0.5f }
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

		// Bind vertex buffer
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		// Set primitive topology
		m_dxo.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind input buffer layout
		m_dxo.context->IASetInputLayout(m_inputLayout.Get());

		// Bind render target
		m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), nullptr);

		m_shader->Bind();

		// Draw the triangle
		m_dxo.context->Draw(3u, 0u);
	}

	void BasicTriangle::ImGuiUpdate() {
	}
}
