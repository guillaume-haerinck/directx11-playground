#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "components/graphics/Material.h"

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	TexturedPrimitives::TexturedPrimitives(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		auto entity = m_ctx.registry.create();

		// Shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/TexturedPrimitives_VS.cso");
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/TexturedPrimitives_PS.cso");
		m_VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(VSConstantBuffer0)));
		VShader.constantBuffers.push_back(m_VSCB0);
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);

		// Material
		comp::PhongMaterial material = {};
		auto texture = m_ctx.rcommand->CreateTexture(comp::PhongTexSlot::DIFFUSE, L"res/textures/test.jpg");
		material.textures.push_back(texture);
		m_ctx.registry.assign<comp::PhongMaterial>(entity, material);

		// Mesh
		auto mesh = primFactory.CreateUVSphere();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
	}

	TexturedPrimitives::~TexturedPrimitives() {
	}

	void TexturedPrimitives::Update() {
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

		// Update systems
		m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader, comp::PhongMaterial>()
			.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader, comp::PhongMaterial& material) {
			m_ctx.rcommand->BindVertexShader(VShader);
			m_ctx.rcommand->BindPixelShader(PShader);
			m_ctx.rcommand->BindVertexBuffer(mesh.vb);

			for (auto texture : material.textures) {
				m_ctx.rcommand->BindTexture(texture);
			}

			m_ctx.rcommand->BindIndexBuffer(mesh.ib);
			m_ctx.rcommand->DrawIndexed(mesh.ib.count);
		});
	}

	void TexturedPrimitives::ImGuiUpdate() {
	}
}
