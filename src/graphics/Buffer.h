#pragma once

#include "DXObjects.h"

class VertexBuffer {
public:
	// TODO takes vertices, size and stride ? (layout is handled by shader ?)
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


class ConstantBuffer {
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Bind();
	void Unbind();

private:

};


class TextureBuffer {
public:
	TextureBuffer();
	~TextureBuffer();

	void Bind();
	void Unbind();

private:

};



