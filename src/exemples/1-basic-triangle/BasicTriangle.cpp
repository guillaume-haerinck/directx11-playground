#include "pch.h"
#include "BasicTriangle.h"

#include "graphics/DXException.h"
#include "components/graphics/Mesh.h"
#include "components/graphics/Shader.h"

namespace exemple {
	BasicTriangle::BasicTriangle(Context& context) : m_ctx(context) {
		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(ied, ARRAYSIZE(ied), L"BasicTriangleVS.cso");
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"BasicTrianglePS.cso");

		// Vertex buffer
		XMFLOAT2 vertices[] = {
			{ XMFLOAT2(0.0f,  0.5f) },
			{ XMFLOAT2(0.5f, -0.5f) },
			{ XMFLOAT2 (-0.5f, -0.5f) }
		};
		comp::VertexBuffer vertexBuffer = m_ctx.rcommand->CreateVertexBuffer(vertices, sizeof(vertices), sizeof(XMFLOAT2));

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, vertexBuffer);
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
			.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
			 m_ctx.rcommand->BindVertexShader(VShader);
			 m_ctx.rcommand->BindPixelShader(PShader);
			 m_ctx.rcommand->BindVertexBuffer(mesh.vb);
			 m_ctx.rcommand->Draw(mesh.vb.count);
		});
	}

	void BasicTriangle::ImGuiUpdate() {
	}
}
