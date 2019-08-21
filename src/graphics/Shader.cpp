#include "pch.h"
#include "Shader.h"

#include "DXException.h"

Shader::Shader(DXObjects dxObjects, std::vector<D3D11_INPUT_ELEMENT_DESC>* ied, LPCWSTR vsFilePath, LPCWSTR psFilePath) : m_dxo(dxObjects)
{
	// Create vertex shader
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(vsFilePath, &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateVertexShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &m_vertexShader
		)
	);

	// Create input buffer layout
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateInputLayout(
			ied->data(), ied->size(),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&m_inputLayout
		)
	);
	
	// Create pixel shader
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(psFilePath, &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreatePixelShader(
			blob->GetBufferPointer(), blob->GetBufferSize(),
			nullptr, &m_pixelShader
		)
	);
}

Shader::~Shader() {
}

void Shader::Bind() const {
	m_dxo.context->IASetInputLayout(m_inputLayout.Get());
	m_dxo.context->PSSetShader(m_pixelShader.Get(), nullptr, 0u);
	m_dxo.context->VSSetShader(m_vertexShader.Get(), nullptr, 0u);
}

void Shader::Unbind() const {
	// TODO
}
