#pragma once

#include "DXObjects.h"

class Shader {
public:
	Shader(DXObjects dxObjects, const char* vsFilePath, const char* psFilePath);
	~Shader();

	void Bind();
	void Unbind();

private:
	DXObjects m_dxo;
};
