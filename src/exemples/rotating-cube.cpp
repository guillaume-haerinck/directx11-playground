#include "pch.h"
#include "rotating-cube.h"

#include "graphics/DXException.h"

namespace exemple {
	RotatingCube::RotatingCube(DXObjects dxObjects) : m_dxo(dxObjects)
	{
		///////////////////// SHADER & INPUT BUFFER & CONSTANT BUFFER

		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		m_shader = std::make_unique<Shader>(m_dxo, ied, ARRAYSIZE(ied), L"basicVS.cso", L"basicPS.cso");
		m_shader->AddVSConstantBuffer(sizeof(ConstantBufferRect));

		/////////////////// VERTEX BUFFER

		const Vertex vertices[] = {
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Lower left
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Lower right	
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Top left
			{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Top right

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Lower left
			{ XMFLOAT3( 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Lower right	
			{ XMFLOAT3(-1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Top left
			{ XMFLOAT3( 1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }	// Back Top right
		};
		
		// Create vertex buffer
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateBuffer(&bd, &sd, &m_vertexBuffer)
		);

		/////////////////// INDEX BUFFER

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

		// Set primitive topology
		m_dxo.context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind render target
		m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), m_dxo.depthStencilView.Get());

		m_shader->Bind();

		// Update constant buffer
		ConstantBufferRect cb;
		XMStoreFloat4x4(&cb.matGeo, XMMatrixIdentity());

		XMMATRIX view = XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

		// DirectXMaths matrix are Row major and HLSL are Column major
		// So we must use the transpose matrix
		XMStoreFloat4x4(&cb.matVP, XMMatrixTranspose(view));
		m_shader->UpdateVSConstantBuffer(0, &cb);

		// Bind vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		m_indexBuffer->Bind();

		// Draw the triangle
		m_dxo.context->DrawIndexed(36u, 0u, 0);
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
