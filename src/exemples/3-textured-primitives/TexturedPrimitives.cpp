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

		Vertex vertices[] = {
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f) }, // Front Lower left
			{ XMFLOAT3(1.0f, -1.0f, -1.0f) }, // Front Lower right	
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f) }, // Front Top left
			{ XMFLOAT3(1.0f,  1.0f, -1.0f) }, // Front Top right

			{ XMFLOAT3(-1.0f, -1.0f,  1.0f) }, // Back Lower left
			{ XMFLOAT3(1.0f, -1.0f,  1.0f) }, // Back Lower right	
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f) }, // Back Top left
			{ XMFLOAT3(1.0f,  1.0f,  1.0f) }  // Back Top right
		};
		m_vertexBuffer = std::make_unique<VertexBuffer>(m_dxo, vertices, ARRAYSIZE(vertices), sizeof(Vertex));

		// Index buffer
		WORD indices[] = {
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
		m_indexBuffer = std::make_unique<IndexBuffer>(m_dxo, indices, ARRAYSIZE(indices));
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
		m_dxo.context->DrawIndexedInstanced(m_indexBuffer->GetCount(), 3u, 0u, 0, 0u);
	}
	void TexturedPrimitives::ImGuiUpdate() {
	}
}
