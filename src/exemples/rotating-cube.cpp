#include "pch.h"
#include "rotating-cube.h"

#include "graphics/DXException.h"

namespace exemple {
	RotatingCube::RotatingCube(DXObjects dxObjects) : m_dxo(dxObjects)
	{
		m_shader = std::make_unique<Shader>(m_dxo, L"basicVS.cso", L"basicPS.cso");

		struct Vertex {
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT4 Color;
		};

		const Vertex vertices[] = {
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },	// Lower left
			{ DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },	// Top left
			{ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Lower right
			{ DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }		// Top right
		};

		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// Create input buffer layout
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateInputLayout(
				ied, ARRAYSIZE(ied),
				m_shader->GetVertexShaderBlob()->GetBufferPointer(),
				m_shader->GetVertexShaderBlob()->GetBufferSize(),
				&m_inputLayout
			)
		);

		/////////////////// VERTEX BUFFER

		/*


		Vertex vertices[] = {
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3( 1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }
		};
		*/

		
		// General temp data
		D3D11_SUBRESOURCE_DATA sd = {};

		// Create vertex buffer
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);
		sd.pSysMem = vertices;
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateBuffer(&bd, &sd, &m_vertexBuffer)
		);

		// Bind vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		
		/////////////////// INDEX BUFFER

		/*
		WORD indices[] = { // Size is 36
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
		};
		*/

		WORD indices[] = {
			0, 1, 2,
			1, 3, 2
		};

		// Create Index Buffer
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(indices);
		bd.StructureByteStride = sizeof(WORD);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		sd.pSysMem = indices;
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateBuffer(&bd, &sd, &m_indexBuffer)
		);

		// Bind Index buffer
		m_dxo.context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		//////////////////// CONSTANT BUFFER

		// Create the constant buffer
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateBuffer(&bd, nullptr, &m_constantBuffer)
		);
	}

	RotatingCube::~RotatingCube() {
	}

	void RotatingCube::Update() {

		// Update constant buffer
		ConstantBuffer cb;
		cb.World = DirectX::XMMatrixIdentity();
		DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
		DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		cb.View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
		cb.Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 800 / (FLOAT)600, 0.01f, 100.0f);
		m_dxo.context->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
		m_dxo.context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

		// Set primitive topology
		m_dxo.context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind input buffer layout
		m_dxo.context->IASetInputLayout(m_inputLayout.Get());

		// Bind render target
		m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), nullptr);

		m_shader->Bind();

		// Draw the triangle
		m_dxo.context->DrawIndexed(6u, 0u, 0);
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
