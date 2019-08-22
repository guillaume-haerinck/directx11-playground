#pragma once

#include "DXObjects.h"

/**
 * @brief Send model data to the GPU and keep a reference to it
 */
class VertexBuffer {
public:
	/**
	 * @brief Create the buffer to the directX API
	 *
	 * @param dxObjects - DirectX common Objects
	 * @param vertices - Pointer to the array of vertices
	 * @param count - The number of vertices in the array
	 * @param structureByteStride - The size in bytes of one vertex
	 */
	VertexBuffer(DXObjects dxObjects, void* vertices, unsigned int count, unsigned int structureByteStride);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_count;  }

private:
	DXObjects m_dxo;
	unsigned int m_stride;
	unsigned int m_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};


/**
 * @brief Used to prevent duplication of vertices for the same model
 */
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
