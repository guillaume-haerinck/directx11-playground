#pragma once

#include "DXObjects.h"

class VertexBuffer {
public:
	// TODO takes vertices, size and stride ? (layout is handled by shader ?)
	// What about write access ? And multiple buffers for the same object ?
	VertexBuffer(DXObjects dxObjects);
	~VertexBuffer();

	void Bind();
	void Unbind();

private:
	DXObjects m_dxObjects;
};


class IndexBuffer {
public:
	IndexBuffer(DXObjects dxObjects, WORD* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	
	unsigned int GetCount() const { return m_count; }

private:
	DXObjects m_dxo;
	unsigned int m_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
};
