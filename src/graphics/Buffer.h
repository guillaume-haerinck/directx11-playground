#pragma once

#include "DXObjects.h"

class VertexBuffer {
public:
	VertexBuffer(DXObjects dxObjects);
	~VertexBuffer();

	void Bind();
	void UnBind();

private:
	DXObjects m_dxObjects;
};


class IndexBuffer {
public:
	IndexBuffer(DXObjects dxObjects);
	~IndexBuffer();

	void Bind();
	void UnBind();

private:
	DXObjects m_dxo;
};

