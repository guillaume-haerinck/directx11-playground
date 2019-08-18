#pragma once

#include "DXObjects.h"

class Shader {
public:
	Shader(DXObjects dxObjects, LPCWSTR vsFilePath, LPCWSTR psFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Used to setup Input layout
	Microsoft::WRL::ComPtr<ID3DBlob> GetVertexShaderBlob() const { return m_vsShaderBlob; }

private:
	DXObjects m_dxo;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsShaderBlob;
};
