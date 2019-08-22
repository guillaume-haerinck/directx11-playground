#include "pch.h"
#include "Buffer.h"

#include "DXException.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// VERTEX BUFFER //////////////////////////////
///////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(DXObjects& dxObjects, void* vertices, unsigned int count, unsigned int structureByteStride)
	: m_dxo(dxObjects), m_stride(structureByteStride), m_count(count)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = structureByteStride * count;
	bd.StructureByteStride = structureByteStride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, &sd, &m_vertexBuffer)
	);
}

VertexBuffer::~VertexBuffer() {
}

void VertexBuffer::Bind() const {
	UINT stride = m_stride;
	UINT offset = 0;
	m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);
}

void VertexBuffer::Unbind() const {
}

///////////////////////////////////////////////////////////////////////////
/////////////////////////////// INDEX BUFFER //////////////////////////////
///////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(DXObjects& dxObjects, WORD* indices, unsigned int count)
	: m_dxo(dxObjects), m_count(count)
{
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * count;
	bd.StructureByteStride = sizeof(WORD);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = indices;

	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, &sd, &m_indexBuffer)
	);
}

IndexBuffer::~IndexBuffer() {
}

void IndexBuffer::Bind() const {
	m_dxo.context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void IndexBuffer::Unbind() const {

}
