#include "pch.h"
#include "RenderCommand.h"

#include "DXException.h"

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

ID3D11Buffer* RenderCommand::CreateVertexBuffer(void* vertices, unsigned int byteWidth, unsigned int stride) const {
	ID3D11Buffer* vertexBuffer;

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = byteWidth;
	bd.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, &sd, &vertexBuffer)
	);

	return vertexBuffer;
}

ID3D11Buffer* RenderCommand::CreateIndexBuffer(WORD* indices, unsigned int count) const {
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

	return indexBuffer;
}

ID3D11Buffer* RenderCommand::CreateConstantBuffer(unsigned int byteWidth) const {
	ID3D11Buffer* constantBuffer = nullptr;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, nullptr, &constantBuffer)
	);

	return constantBuffer;
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

	return std::make_tuple(vertexShader, inputLayout);
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

void RenderCommand::BindVertexBuffer(ID3D11Buffer* buffer, unsigned int stride) const {
	UINT offset = 0;
	m_dxo.context->IASetVertexBuffers(0u, 1u, &buffer, &stride, &offset);
}

void RenderCommand::BindIndexBuffer(ID3D11Buffer* buffer) const {
	m_dxo.context->IASetIndexBuffer(buffer, DXGI_FORMAT_R16_UINT, 0);
}

void RenderCommand::BindVertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* layout) const {
	m_dxo.context->IASetInputLayout(layout);
	m_dxo.context->VSSetShader(shader, nullptr, 0u);
}

void RenderCommand::BindVSConstantBuffer(ID3D11Buffer* buffer, unsigned int slot) const {
	m_dxo.context->VSSetConstantBuffers(slot, 1, &buffer);
}

void RenderCommand::BindPixelShader(ID3D11PixelShader* shader) const {
	m_dxo.context->PSSetShader(shader, nullptr, 0u);
}

void RenderCommand::BindPSConstantBuffer(ID3D11Buffer* buffer, unsigned int slot) const {
	m_dxo.context->PSSetConstantBuffers(slot, 1, &buffer);
}

void RenderCommand::UpdateConstantBuffer(ID3D11Buffer* buffer, void* data, unsigned int size) const {
	D3D11_MAPPED_SUBRESOURCE msr;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.context->Map(
			buffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &msr
		)
	);
	memcpy(msr.pData, data, size);
	m_dxo.context->Unmap(buffer, 0u);
}
