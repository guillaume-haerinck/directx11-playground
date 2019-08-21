#pragma once

#include "DXObjects.h"

class VertexBuffer {
public:
	// TODO what about multiple buffer for the same object ?
	// Use a function to add a buffer ? Or use another object ?

	/**
	 * @param dxObjects -
	 * @param vertices -
	 * @param count - The number of vertices
	 * @param structureByteStride - The size in bytes of one vertex
	 */
	VertexBuffer(DXObjects dxObjects, void* vertices, unsigned int count, unsigned int structureByteStride);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	DXObjects m_dxo;
	unsigned int m_stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
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
