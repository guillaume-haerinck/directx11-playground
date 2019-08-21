#pragma once

#include "DXObjects.h"

// TODO create a class for geometry shader
// TODO create a class for tesselation stage

class Shader {
public:
	// TODO create hashmap from inputBuffer to reuse it if already exist
	Shader(DXObjects dxObjects, std::vector<D3D11_INPUT_ELEMENT_DESC>* ied, LPCWSTR vsFilePath, LPCWSTR psFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// TODO function to setup constant buffer (like uniforms in OGL) ? Set and update ?
	// What about the type when a shader has multiple constant buffers ?

private:
	DXObjects m_dxo;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};
