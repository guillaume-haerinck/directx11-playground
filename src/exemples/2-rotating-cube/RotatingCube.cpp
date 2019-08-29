#include "pch.h"
#include "RotatingCube.h"

#include "factories/PrimitiveFactory.h"
#include "components/graphics/Mesh.h"
#include "systems/RenderSystem.h"

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	struct PSConstantBuffer0 {
		XMFLOAT4 color[6];
	};

	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		// Init
		PrimitiveFactory primFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Shader
		auto [VShader, inputLayout] = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"RotatingCubeVS.cso");
		auto PShader = m_ctx.rcommand->CreatePixelShader(L"RotatingCubePS.cso");
		m_VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(VSConstantBuffer0)));
		comp::ConstantBuffer PSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(PSConstantBuffer0)));

		// Update PSconstant buffer as it will not change
		PSConstantBuffer0 PSCB0data = {
			XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_ctx.rcommand->UpdateConstantBuffer(PSCB0, &PSCB0data);

		// Save data to entity
		auto entity = m_ctx.registry.create();
		comp::Mesh mesh = primFactory.CreateBox();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader, inputLayout, &m_VSCB0, 1);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader, &PSCB0, 1);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
	}

	RotatingCube::~RotatingCube() {
	}

	void RotatingCube::Update() {
		m_timer.Tick([&](){});

		// DirectXMaths matrix are Row major and HLSL are Column major, so we must use the transpose matrix
		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);
		VSConstantBuffer0 VSCB0data;
		XMStoreFloat4x4(&VSCB0data.matVP, view);
		XMStoreFloat4x4(&VSCB0data.matGeo, XMMatrixIdentity());

		// Update buffer
		// TODO see if constant buffer can be updated in a system
		m_ctx.rcommand->UpdateConstantBuffer(m_VSCB0, &VSCB0data);

		// Update systems
		for (int i = 0; i < m_systems.size(); i++) {
			m_systems.at(i)->Update();
		}
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
