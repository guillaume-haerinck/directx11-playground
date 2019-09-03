#include "pch.h"
#include "ModelLoading.h"

#include "factories/components/ModelFactory.h"

namespace exemple {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		ModelFactory modelFactory(context);

		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(ied, ARRAYSIZE(ied), L"ModelLoadingVS.cso");
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"ModelLoadingPS.cso");

		// VertexBuffer
		comp::VertexBuffer vertexBuffer = modelFactory.CreateModel("res/models/triangle/Triangle.gltf");

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, vertexBuffer);
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
	}

	ModelLoading::~ModelLoading() {
	}

	void ModelLoading::Update() {
		m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
			.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
			m_ctx.rcommand->BindVertexShader(VShader);
			m_ctx.rcommand->BindPixelShader(PShader);
			m_ctx.rcommand->BindVertexBuffer(mesh.vb);
			m_ctx.rcommand->Draw(mesh.vb.count);
		});
	}

	void ModelLoading::ImGuiUpdate() {
	}
}
