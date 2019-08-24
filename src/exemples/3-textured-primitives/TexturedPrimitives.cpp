#include "pch.h"
#include "TexturedPrimitives.h"

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	struct PSConstantBuffer0 {
		XMFLOAT4 color[6];
	};

	TexturedPrimitives::TexturedPrimitives(DXObjects& dxObjects) : m_dxo(dxObjects) {
		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		m_shader = std::make_unique<Shader>(m_dxo, ied, ARRAYSIZE(ied), L"TexturedPrimitivesVS.cso", L"TexturedPrimitivesPS.cso");
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

		// Vertex buffer
		struct Vertex {
			XMFLOAT3 Position;
		};

		m_vertexBuffer = std::make_unique<VertexBuffer>(m_dxo, m_sphere.GetVertices().data(), m_sphere.GetVertices().size(), sizeof(Vertex));
		m_indexBuffer = std::make_unique<IndexBuffer>(m_dxo, m_sphere.GetIndices().data(), m_sphere.GetIndices().size());
	}

	TexturedPrimitives::~TexturedPrimitives() {
	}
	void TexturedPrimitives::Update() {
		m_timer.Tick([&]() {});

		m_shader->Bind();

		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(-3.0f, 0.0f, 6.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);

		// Update VSconstant buffer
		VSConstantBuffer0 vsCB;
		XMStoreFloat4x4(&vsCB.matVP, view);
		XMStoreFloat4x4(&vsCB.matGeo, XMMatrixIdentity());
		m_shader->UpdateVSConstantBuffer(0, &vsCB);

		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		// Draw 3 instanced cubes
		m_dxo.context->DrawIndexed(m_indexBuffer->GetCount(), 0u, 0);
	}
	void TexturedPrimitives::ImGuiUpdate() {
	}
}
