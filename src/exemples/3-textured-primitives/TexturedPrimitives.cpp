#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"
#include "factories/PrimitiveFactory.h"
#include "systems/RenderSystem.h"



namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	TexturedPrimitives::TexturedPrimitives(Context& context) : m_ctx(context) {
		// Init
		PrimitiveFactory primFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Shader
		auto [VShader, inputLayout] = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"TexturedPrimitivesVS.cso");
		auto PShader = m_ctx.rcommand->CreatePixelShader(L"TexturedPrimitivesPS.cso");
		m_VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(VSConstantBuffer0)));

		// Other data
		auto texture = m_ctx.rcommand->CreateTexture(L"res/textures/test.jpg");
		auto mesh = primFactory.CreateUVSphere();
		mesh.textures.push_back(texture);

		// Create entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader, inputLayout, &m_VSCB0, 1);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
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
		for (int i = 0; i < m_systems.size(); i++) {
			m_systems.at(i)->Update();
		}
	}

	void TexturedPrimitives::ImGuiUpdate() {
	}
}
