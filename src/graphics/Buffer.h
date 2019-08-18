#pragma once

#include "DXObjects.h"

class VertexBuffer {
public:
	VertexBuffer(DXObjects dxObjects);
	~VertexBuffer();

	void Bind();
	void Unbind();

private:
	DXObjects m_dxObjects;
};


class IndexBuffer {
public:
	IndexBuffer(DXObjects dxObjects);
	~IndexBuffer();

	void Bind();
	void Unbind();

private:
	DXObjects m_dxo;
};

