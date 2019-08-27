#include "pch.h"
#include "BasicTriangle.h"

#include "graphics/DXException.h"

namespace exemple {
	BasicTriangle::BasicTriangle(DXObjects& dxObjects) : m_dxo(dxObjects) {
		m_rcommand = std::make_unique<RenderCommand>(dxObjects);

		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		auto temp = m_rcommand->CreateVertexShader(ied, ARRAYSIZE(ied), L"BasicTriangleVS.cso");
		m_VSShader = std::get<0>(temp);
		m_inputLayout = std::get<1>(temp);
		m_PSShader = m_rcommand->CreatePixelShader(L"BasicTrianglePS.cso");

		// Vertex buffer
		XMFLOAT2 vertices[] = {
			{ XMFLOAT2(0.0f,  0.5f) },
			{ XMFLOAT2(0.5f, -0.5f) },
			{ XMFLOAT2 (-0.5f, -0.5f) }
		};
		m_vertexBuffer = m_rcommand->CreateVertexBuffer(vertices, sizeof(vertices), sizeof(XMFLOAT2));
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		m_rcommand->BindVertexShader(m_VSShader.Get(), m_inputLayout.Get());
		m_rcommand->BindPixelShader(m_PSShader.Get());
		m_rcommand->BindVertexBuffer(m_vertexBuffer.Get(), sizeof(XMFLOAT2));

		m_dxo.context->Draw(3u, 0u);
	}

	void BasicTriangle::ImGuiUpdate() {
	}
}
