#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"

#include <microsoft-wic-texture-loader/WICTextureLoader.h>

namespace exemple {
	struct VSConstantBuffer0 {
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo;
	};

	TexturedPrimitives::TexturedPrimitives(DXObjects& dxObjects) : m_dxo(dxObjects) {
		// Shader
		m_shader = std::make_unique<Shader>(m_dxo, prim::InputElements, prim::InputElementCount, L"TexturedPrimitivesVS.cso", L"TexturedPrimitivesPS.cso");
		m_shader->AddVSConstantBuffer(sizeof(VSConstantBuffer0));

		// Vertex buffer
		m_vertexBuffer = std::make_unique<VertexBuffer>(m_dxo, m_sphere.GetVertices().data(), m_sphere.GetVertices().size(), prim::InputElementSize);
		m_indexBuffer = std::make_unique<IndexBuffer>(m_dxo, m_sphere.GetIndices().data(), m_sphere.GetIndices().size());
		
		// Texture
		Microsoft::WRL::ComPtr<ID3D11Resource> res;
		DX::ThrowIfFailed(CALL_INFO,
			CreateWICTextureFromFile(m_dxo.device.Get(), m_dxo.context.Get(),  L"res/textures/test.jpg", res.GetAddressOf(), m_srv.GetAddressOf())
		);

		// Bind texture
		m_dxo.context->PSSetShaderResources(0u, 1u, m_srv.GetAddressOf());

		// Set sampler state
		D3D11_SAMPLER_DESC sdesc = {};
		sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateSamplerState(&sdesc, &m_sampler)
		);

		// Bind sampler
		m_dxo.context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
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
