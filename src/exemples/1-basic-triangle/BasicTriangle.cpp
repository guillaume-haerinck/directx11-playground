#include "pch.h"
#include "BasicTriangle.h"

#include "graphics/DXException.h"

namespace exemple {
	BasicTriangle::BasicTriangle(DXObjects& dxObjects) : m_dxo(dxObjects) {
		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_shader = std::make_unique<Shader>(m_dxo, ied, ARRAYSIZE(ied), L"BasicTriangleVS.cso", L"BasicTrianglePS.cso");

		struct Vertex {
			XMFLOAT2 Position;
		};

		// Vertex buffer
		Vertex vertices[] = {
			{ XMFLOAT2(0.0f,  0.5f) },
			{ XMFLOAT2(0.5f, -0.5f) },
			{ XMFLOAT2 (-0.5f, -0.5f) }
		};
		m_vertexBuffer = std::make_unique<VertexBuffer>(m_dxo, vertices, ARRAYSIZE(vertices), sizeof(Vertex));
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		m_shader->Bind();

		m_vertexBuffer->Bind();

		m_dxo.context->Draw(m_vertexBuffer->GetCount(), 0u);
	}

	void BasicTriangle::ImGuiUpdate() {
	}
}
