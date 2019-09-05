#include "pch.h"
#include "RenderCommand.h"

#include <microsoft-wic-texture-loader/WICTextureLoader.h>

#include "DXException.h"

RenderCommand::RenderCommand(DXObjects& dxObjects) 
	: m_dxo(dxObjects), m_lastPShaderBound(nullptr), m_lastVShaderBound(nullptr)
{}

void RenderCommand::Clear() const {
	const float color[] = { 0.0f, 0.5f, 1.0f, 1.0f };
	m_dxo.context->ClearRenderTargetView(m_dxo.target.Get(), color);
	m_dxo.context->ClearDepthStencilView(m_dxo.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void RenderCommand::Swap() const {
	m_dxo.swapChain->Present(1u, 0u);
}

comp::AttributeBuffer RenderCommand::CreateAttributeBuffer(void* vertices, unsigned int count, unsigned int stride) const {
	ID3D11Buffer* attributeBuffer;

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = count * stride;
	bd.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, &sd, &attributeBuffer)
	);

	comp::AttributeBuffer ab = {};
	ab.buffer = attributeBuffer;
	ab.stride = stride;
	ab.count = count;
	ab.byteWidth = count * stride;
	return ab;
}

comp::IndexBuffer RenderCommand::CreateIndexBuffer(void* indices, unsigned int count) const {
	ID3D11Buffer* indexBuffer;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * count;
	bd.StructureByteStride = sizeof(WORD);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = indices;

	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, &sd, &indexBuffer)
	);

	comp::IndexBuffer ib = {};
	ib.buffer = indexBuffer;
	ib.count = count;
	return ib;
}

comp::ConstantBuffer RenderCommand::CreateConstantBuffer(unsigned int slot, unsigned int byteWidth) const {
	ID3D11Buffer* constantBuffer = nullptr;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, nullptr, &constantBuffer)
	);

	comp::ConstantBuffer cb = {};
	cb.buffer = constantBuffer;
	cb.slot = slot;
	cb.byteWidth = byteWidth;
	return cb;
}

comp::Sampler RenderCommand::CreateSampler(comp::SamplerSlot slot) const {
	Microsoft::WRL::ComPtr<ID3D11SamplerState> splr;
	D3D11_SAMPLER_DESC sdesc = {};
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateSamplerState(&sdesc, &splr)
	);

	comp::Sampler sampler = {};
	sampler.sampler = splr;
	sampler.slot = slot;
	return sampler;
}

comp::Texture RenderCommand::CreateTexture(unsigned int slot, LPCWSTR filepath, comp::SamplerSlot samplerSlot) const {
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(CALL_INFO,
		CreateWICTextureFromFile(m_dxo.device.Get(), m_dxo.context.Get(), filepath, res.GetAddressOf(), srv.GetAddressOf())
	);

	comp::Texture texture = {};
	texture.srv = srv;
	texture.slot = slot;
	texture.samplerSlot = samplerSlot;
	return texture;
}

comp::VertexShader RenderCommand::CreateVertexShader(D3D11_INPUT_ELEMENT_DESC* iedArray, unsigned int iedElementCount, LPCWSTR filePath) const {
	ID3D11VertexShader* vertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(filePath, &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateVertexShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &vertexShader
		)
	);

	ID3D11InputLayout* inputLayout;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateInputLayout(
			iedArray, iedElementCount,
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout
		)
	);

	comp::VertexShader shader = {};
	shader.layout = inputLayout;
	shader.shader = vertexShader;
	return shader;
}

comp::PixelShader RenderCommand::CreatePixelShader(LPCWSTR filePath) const {
	ID3D11PixelShader* pixelShader;

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(filePath, &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreatePixelShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &pixelShader
		)
	);

	comp::PixelShader shader = {};
	shader.shader = pixelShader;
	return shader;
}

void RenderCommand::BindVertexBuffer(comp::VertexBuffer vb) const {
	m_dxo.context->IASetVertexBuffers(0u, vb.buffers.size(), vb.buffers.data()->GetAddressOf(), vb.strides.data(), vb.offsets.data());
}

void RenderCommand::BindIndexBuffer(comp::IndexBuffer ib) const {
	m_dxo.context->IASetIndexBuffer(ib.buffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void RenderCommand::BindSampler(comp::Sampler sampler) const {
	m_dxo.context->PSSetSamplers(sampler.slot, 1, sampler.sampler.GetAddressOf());
}

void RenderCommand::BindTexture(comp::Texture texture) const {
	m_dxo.context->PSSetShaderResources(texture.slot, 1u, texture.srv.GetAddressOf());
}

void RenderCommand::BindVertexShader(comp::VertexShader vs) {
	if (m_lastVShaderBound != vs.shader.Get()) {
		m_lastVShaderBound = vs.shader.Get();
		m_dxo.context->IASetInputLayout(vs.layout.Get());
		m_dxo.context->VSSetShader(vs.shader.Get(), nullptr, 0u);

		// TODO bind all at once (see how to ensure that index is unique and in the right order)
		// maybe use the index of the vector as start
		for (auto cb : vs.constantBuffers) {
			m_dxo.context->VSSetConstantBuffers(cb.slot, 1, cb.buffer.GetAddressOf());
		}
	}
}

void RenderCommand::BindPixelShader(comp::PixelShader ps) {
	if (m_lastPShaderBound != ps.shader.Get()) {
		m_lastPShaderBound = ps.shader.Get();
		m_dxo.context->PSSetShader(ps.shader.Get(), nullptr, 0u);

		for (auto cb : ps.constantBuffers) {
			m_dxo.context->PSSetConstantBuffers(cb.slot, 1, cb.buffer.GetAddressOf());
		}
	}
}

void RenderCommand::UpdateConstantBuffer(comp::ConstantBuffer cb, void* data) const {
	D3D11_MAPPED_SUBRESOURCE msr;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.context->Map(
			cb.buffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &msr
		)
	);
	memcpy(msr.pData, data, cb.byteWidth);
	m_dxo.context->Unmap(cb.buffer.Get(), 0u);
}

void RenderCommand::Draw(unsigned int count) const {
	m_dxo.context->Draw(count, 0u);
}

void RenderCommand::DrawIndexed(unsigned int count) const {
	m_dxo.context->DrawIndexed(count, 0u, 0);
}
