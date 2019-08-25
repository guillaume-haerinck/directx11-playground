#include "pch.h"
#include "RotatingCube.h"

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	struct PSConstantBuffer0 {
		XMFLOAT4 color[6];
	};

	RotatingCube::RotatingCube(DXObjects& dxObjects) : m_dxo(dxObjects)
	{
		// Shader
		m_shader = std::make_unique<Shader>(m_dxo, prim::InputElements, prim::InputElementCount, L"RotatingCubeVS.cso", L"RotatingCubePS.cso");
		m_shader->AddVSConstantBuffer(sizeof(VSConstantBuffer0));
		m_shader->AddPSConstantBuffer(sizeof(PSConstantBuffer0));

		// Update PSconstant buffer as it will not change
		PSConstantBuffer0 psCB = {
			XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_shader->UpdatePSConstantBuffer(0, &psCB);

		m_vertexBuffer = std::make_unique<VertexBuffer>(m_dxo, m_box.GetVertices().data(), m_box.GetVertices().size(), prim::InputElementSize);
		m_indexBuffer = std::make_unique<IndexBuffer>(m_dxo, m_box.GetIndices().data(), m_box.GetIndices().size());
	}

	RotatingCube::~RotatingCube() {
	}

	void RotatingCube::Update() {
		m_timer.Tick([&](){});

		m_shader->Bind();

		// DirectXMaths matrix are Row major and HLSL are Column major, so we must use the transpose matrix
		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);

		// Update VSconstant buffer
		VSConstantBuffer0 vsCB;
		XMStoreFloat4x4(&vsCB.matVP, view);
		XMStoreFloat4x4(&vsCB.matGeo, XMMatrixIdentity());
		m_shader->UpdateVSConstantBuffer(0, &vsCB);

		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		// Draw the cube
		m_dxo.context->DrawIndexed(m_indexBuffer->GetCount(), 0u, 0);
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
