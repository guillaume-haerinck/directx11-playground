#include "pch.h"
#include "ModelLoading.h"

#include "factories/components/ModelFactory.h"

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		ModelFactory modelFactory(context);

		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(ied, ARRAYSIZE(ied), L"ModelLoadingVS.cso");
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"ModelLoadingPS.cso");
		m_VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(VSConstantBuffer0)));
		VShader.constantBuffers.push_back(m_VSCB0);

		// VertexBuffer
		comp::Model model = modelFactory.CreateModel("res/models/cube/Cube.gltf");

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Model>(entity, model);
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
	}

	ModelLoading::~ModelLoading() {
	}

	void ModelLoading::Update() {
		m_timer.Tick([&]() {});

		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 6.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);

		// Update VSconstant buffer
		VSConstantBuffer0 VSCB0data;
		XMStoreFloat4x4(&VSCB0data.matVP, view);
		XMStoreFloat4x4(&VSCB0data.matGeo, XMMatrixIdentity());
		m_ctx.rcommand->UpdateConstantBuffer(m_VSCB0, &VSCB0data);

		m_ctx.registry.view<comp::Model, comp::VertexShader, comp::PixelShader>()
			.each([&](comp::Model& model, comp::VertexShader& VShader, comp::PixelShader& PShader) {
			m_ctx.rcommand->BindVertexShader(VShader);
			m_ctx.rcommand->BindPixelShader(PShader);
			m_ctx.rcommand->BindVertexBuffer(model.vb);
			m_ctx.rcommand->BindIndexBuffer(model.ib);
			m_ctx.rcommand->DrawIndexed(model.ib.count);
		});
	}

	void ModelLoading::ImGuiUpdate() {
	}
}
