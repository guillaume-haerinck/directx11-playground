#include "pch.h"
#include "RotatingCube.h"

#include "factories/PrimitiveFactory.h"
#include "components/graphics/Shader.h"
#include "components/graphics/Mesh.h"

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	struct PSConstantBuffer0 {
		XMFLOAT4 color[6];
	};

	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		PrimitiveFactory primFactory(context);

		// Shader
		auto [VShader, inputLayout] = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"RotatingCubeVS.cso");
		auto PShader = m_ctx.rcommand->CreatePixelShader(L"RotatingCubePS.cso");
		auto VSCB0 = m_ctx.rcommand->CreateConstantBuffer((sizeof(VSConstantBuffer0)));
		auto PSCB0 = m_ctx.rcommand->CreateConstantBuffer((sizeof(PSConstantBuffer0)));

		// Update PSconstant buffer as it will not change
		PSConstantBuffer0 PSCB0data = {
			XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_ctx.rcommand->UpdateConstantBuffer(PSCB0, &PSCB0data, sizeof(PSCB0data));

		// Save data to entity
		auto entity = primFactory.CreateBox();
		comp::ConstantBuffer VSCB0comp = { VSCB0, 0, sizeof(VSCB0) };
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader, inputLayout, &VSCB0comp, 1);
		comp::ConstantBuffer PSCB0comp = { PSCB0, 0, sizeof(PSCB0) };
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader, &PSCB0comp, 1);
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

		m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
			.each([&, VSCB0data](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
			m_ctx.rcommand->BindVertexShader(VShader.shader.Get(), VShader.layout.Get());
			m_ctx.rcommand->BindVSConstantBuffer(VShader.constantBuffers.at(0).buffer.Get(), VShader.constantBuffers.at(0).slot);
			m_ctx.rcommand->UpdateConstantBuffer(VShader.constantBuffers.at(0).buffer.Get(), (void*) &VSCB0data, VShader.constantBuffers.at(0).byteWidth);

			m_ctx.rcommand->BindPixelShader(PShader.shader.Get());
			m_ctx.rcommand->BindPSConstantBuffer(PShader.constantBuffers.at(0).buffer.Get(), PShader.constantBuffers.at(0).slot);

			m_ctx.rcommand->BindVertexBuffer(mesh.vertexBuffer.Get(), mesh.VBStride);
			m_ctx.rcommand->BindIndexBuffer(mesh.indexBuffer.Get());
			m_ctx.rcommand->DrawIndexed(mesh.IBCount);
		});
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
