#include "pch.h"
#include "BasicTriangle.h"

#include "graphics/DXException.h"
#include "components/graphics/Mesh.h"
#include "components/graphics/Shader.h"

namespace exemple {
	BasicTriangle::BasicTriangle(DXObjects& dxObjects) : m_dxo(dxObjects) {
		m_rcommand = std::make_unique<RenderCommand>(dxObjects);

		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		auto [VSShader, inputLayout] = m_rcommand->CreateVertexShader(ied, ARRAYSIZE(ied), L"BasicTriangleVS.cso");
		auto PSShader = m_rcommand->CreatePixelShader(L"BasicTrianglePS.cso");

		// Vertex buffer
		XMFLOAT2 vertices[] = {
			{ XMFLOAT2(0.0f,  0.5f) },
			{ XMFLOAT2(0.5f, -0.5f) },
			{ XMFLOAT2 (-0.5f, -0.5f) }
		};
		auto vertexBuffer = m_rcommand->CreateVertexBuffer(vertices, sizeof(vertices), sizeof(XMFLOAT2));

		// Assign data to an entity
		auto entity = registry.create();
		registry.assign<comp::Mesh>(entity, vertexBuffer, sizeof(XMFLOAT2));
		registry.assign<comp::VertexShader>(entity, VSShader, inputLayout);
		registry.assign<comp::PixelShader>(entity, PSShader);
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>().each([&](auto& mesh, auto& VSShader, auto& PSShader) {
			m_rcommand->BindVertexShader(VSShader.shader.Get(), VSShader.layout.Get());
			m_rcommand->BindPixelShader(PSShader.shader.Get());
			m_rcommand->BindVertexBuffer(mesh.vertexBuffer.Get(), mesh.VBStride);

			m_dxo.context->Draw(3u, 0u);
		});
	}

	void BasicTriangle::ImGuiUpdate() {
	}
}
