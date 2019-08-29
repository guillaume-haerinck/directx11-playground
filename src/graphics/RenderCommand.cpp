#include "pch.h"
#include "RenderCommand.h"

#include "DXException.h"

ID3D11VertexShader* RenderCommand::m_lastVShaderBound = nullptr;
ID3D11PixelShader* RenderCommand::m_lastPShaderBound = nullptr;

RenderCommand::RenderCommand(DXObjects& dxObjects) : m_dxo(dxObjects)
{
	
}

void RenderCommand::Clear() const {
	const float color[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	m_dxo.context->ClearRenderTargetView(m_dxo.target.Get(), color);
	m_dxo.context->ClearDepthStencilView(m_dxo.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void RenderCommand::Swap() const {
	m_dxo.swapChain->Present(1u, 0u);
}

comp::VertexBuffer RenderCommand::CreateVertexBuffer(void* vertices, unsigned int count, unsigned int stride) const {
	ID3D11Buffer* vertexBuffer;

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
		m_dxo.device->CreateBuffer(&bd, &sd, &vertexBuffer)
	);

	comp::VertexBuffer vb = {};
	vb.buffer = vertexBuffer;
	vb.stride = stride;
	vb.count = count;
	vb.byteWidth = count * stride;
	return vb;
}

comp::IndexBuffer RenderCommand::CreateIndexBuffer(WORD* indices, unsigned int count) const {
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

std::tuple<ID3D11VertexShader*, ID3D11InputLayout*> RenderCommand::CreateVertexShader(D3D11_INPUT_ELEMENT_DESC* ied, unsigned int iedElementCount, LPCWSTR filePath) const {
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
			ied, iedElementCount,
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout
		)
	);

	return { vertexShader, inputLayout };
}

ID3D11PixelShader* RenderCommand::CreatePixelShader(LPCWSTR filePath) const {
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

	return pixelShader;
}

void RenderCommand::BindVertexBuffer(comp::VertexBuffer vb) const {
	UINT offset = 0;
	m_dxo.context->IASetVertexBuffers(0u, 1u, vb.buffer.GetAddressOf(), &vb.stride, &offset);
}

void RenderCommand::BindIndexBuffer(comp::IndexBuffer ib) const {
	m_dxo.context->IASetIndexBuffer(ib.buffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void RenderCommand::BindVertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* layout) {
	if (m_lastVShaderBound != shader) {
		m_lastVShaderBound = shader;
		m_dxo.context->IASetInputLayout(layout);
		m_dxo.context->VSSetShader(shader, nullptr, 0u);
	}
}

void RenderCommand::BindVSConstantBuffer(comp::ConstantBuffer cb) const {
	m_dxo.context->VSSetConstantBuffers(cb.slot, 1, cb.buffer.GetAddressOf());
}

void RenderCommand::BindPixelShader(ID3D11PixelShader* shader) {
	if (m_lastPShaderBound != shader) {
		m_lastPShaderBound = shader;
		m_dxo.context->PSSetShader(shader, nullptr, 0u);
	}
}

void RenderCommand::BindPSConstantBuffer(comp::ConstantBuffer cb) const {
	m_dxo.context->PSSetConstantBuffers(cb.slot, 1, cb.buffer.GetAddressOf());
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
