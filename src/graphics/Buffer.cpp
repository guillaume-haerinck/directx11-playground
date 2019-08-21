#include "pch.h"
#include "Buffer.h"

#include "DXException.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// VERTEX BUFFER //////////////////////////////
///////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(DXObjects dxObjects) {
}

VertexBuffer::~VertexBuffer() {
}

///////////////////////////////////////////////////////////////////////////
/////////////////////////////// INDEX BUFFER //////////////////////////////
///////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(DXObjects dxObjects, WORD* indices, unsigned int size) : m_dxo(dxObjects)
{
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * size;
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
