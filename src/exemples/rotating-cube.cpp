#include "pch.h"
#include "rotating-cube.h"

#include "graphics/DXException.h"

namespace exemple {
	RotatingCube::RotatingCube(DXObjects dxObjects) : m_dxo(dxObjects)
	{
		m_shader = std::make_unique<Shader>(m_dxo, L"basicVS.cso", L"basicPS.cso");

		///////////////////// INPUT BUFFER

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

		const Vertex vertices[] = {
			{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },	// Back Lower left
			{ XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },	// Back Top left
			{ XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },	// Back Lower right
			{ XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },	// Back Top right

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Front Lower left
			{ XMFLOAT3(-1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Front Top left
			{ XMFLOAT3( 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Front Lower right
			{ XMFLOAT3( 1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }	// Front Top right
		};
		
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

		/////////////////// INDEX BUFFER

		WORD indices[] = {
			// Front
			0, 1, 2,
			1, 3, 2,

			// Back
			4, 5, 6,
			5, 7, 6,

			// Top
			5, 1, 7,
			7, 1, 3,

			// Bottom
			4, 0, 6,
			6, 0, 2,

			// Left
			0, 1, 5,
			0, 5, 4,

			// Right
			6, 7, 3,
			6, 3, 2
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
		m_timer.Tick([&](){});

		// Set primitive topology
		m_dxo.context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind render target
		m_dxo.context->OMSetRenderTargets(1u, m_dxo.target.GetAddressOf(), nullptr);

		m_shader->Bind();

		// Update constant buffer
		ConstantBuffer cb;
		XMStoreFloat4x4(&cb.matGeo, XMMatrixIdentity());

		XMVECTOR scaling = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		XMVECTOR rotationOrigin = XMVectorZero();
		XMVECTOR rotationEuler = XMVectorSet(XM_PIDIV4, m_timer.GetFrameCount() * 0.01, 0.0f, 1.0f); // Pitch, Yaw then Roll rotation
		XMVECTOR rotation = XMQuaternionRotationRollPitchYawFromVector(rotationEuler);
		XMVECTOR translation = XMVectorSet(0.0f, 0.0f, 0.3f, 1.0f);
		XMMATRIX view = XMMatrixAffineTransformation(scaling, rotationOrigin, rotation, translation);

		// DirectXMaths matrix are Row major and HLSL are Column major
		// So we must use the transpose matrix
		XMStoreFloat4x4(&cb.matVP, XMMatrixTranspose(view));

		m_dxo.context->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
		m_dxo.context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

		// Bind input buffer layout
		m_dxo.context->IASetInputLayout(m_inputLayout.Get());

		// Bind vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		// Bind Index buffer
		m_dxo.context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		// Draw the triangle
		m_dxo.context->DrawIndexed(36u, 0u, 0);
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
