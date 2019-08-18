#include "pch.h"
#include "Shader.h"

#include "DXException.h"

Shader::Shader(DXObjects dxObjects, LPCWSTR vsFilePath, LPCWSTR psFilePath) : m_dxo(dxObjects)
{
	// Create vertex shader
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(vsFilePath, &m_vsShaderBlob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateVertexShader(
			m_vsShaderBlob->GetBufferPointer(), m_vsShaderBlob->GetBufferSize(),
			nullptr, &m_vertexShader
		)
	);
	
	// Create Pixel shader
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(psFilePath, &m_psShaderBlob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreatePixelShader(
			m_psShaderBlob->GetBufferPointer(), m_psShaderBlob->GetBufferSize(),
			nullptr, &m_pixelShader
		)
	);
}

Shader::~Shader() {
}

void Shader::Bind() {
	m_dxo.context->PSSetShader(m_pixelShader.Get(), nullptr, 0u);
	m_dxo.context->VSSetShader(m_vertexShader.Get(), nullptr, 0u);
}

void Shader::Unbind() {
}
