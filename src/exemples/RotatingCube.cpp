#include "pch.h"
#include "RotatingCube.h"

#include "graphics/DXException.h"

namespace exemple {
	struct ConstantBuffer {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	RotatingCube::RotatingCube(DXObjects dxObjects) : m_dxo(dxObjects)
	{
		// Shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		m_shader = std::make_unique<Shader>(m_dxo, ied, ARRAYSIZE(ied), L"basicVS.cso", L"basicPS.cso");
		m_shader->AddVSConstantBuffer(sizeof(ConstantBuffer));

		// Vertex buffer
		struct Vertex {
			XMFLOAT3 Position;
			XMFLOAT4 Color;
		};

		Vertex vertices[] = {
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Lower left
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Lower right	
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Top left
			{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Top right

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Lower left
			{ XMFLOAT3( 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Lower right	
			{ XMFLOAT3(-1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Top left
			{ XMFLOAT3( 1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }	// Back Top right
		};
		m_vertexBuffer = std::make_unique<VertexBuffer>(m_dxo, vertices, ARRAYSIZE(vertices), sizeof(Vertex));

		// Index buffer
		/* Welding order must be clockwise
						    6_______7
						   /|       |
						  / |      /|
						 /  4_____/_5
						2_______3/ /
						|       | /
						|       |/
						0_______1
		*/
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

		// Update constant buffer
		ConstantBuffer cb;
		XMStoreFloat4x4(&cb.matVP, view);
		XMStoreFloat4x4(&cb.matGeo, XMMatrixIdentity());
		m_shader->UpdateVSConstantBuffer(0, &cb);

		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		// Draw the rectangle
		m_dxo.context->DrawIndexed(m_indexBuffer->GetCount(), 0u, 0);
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
