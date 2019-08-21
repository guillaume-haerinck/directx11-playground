#include "pch.h"
#include "Shader.h"

#include "DXException.h"

Shader::Shader(DXObjects dxObjects, D3D11_INPUT_ELEMENT_DESC* ied, unsigned int iedElementCount, LPCWSTR vsFilePath, LPCWSTR psFilePath) : m_dxo(dxObjects)
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
			ied, iedElementCount,
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

unsigned int Shader::AddVSConstantBuffer(unsigned int byteWidth) {
	ID3D11Buffer* constantBuffer = nullptr;
	
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, nullptr, &constantBuffer)
	);
	
	m_vsConstantBuffers.push_back(constantBuffer);
	unsigned int slot = m_vsConstantBuffers.size() - 1;

	// TODO set all each time there is a new one ?
	m_dxo.context->VSSetConstantBuffers(slot, 1, &constantBuffer);

	return slot;
}

unsigned int Shader::AddPSConstantBuffer(unsigned int byteWidth) {
	ID3D11Buffer* constantBuffer = nullptr;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, nullptr, &constantBuffer)
	);

	m_psConstantBuffers.push_back(constantBuffer);
	unsigned int slot = m_psConstantBuffers.size() - 1;

	// TODO set all each time there is a new one ?
	m_dxo.context->VSSetConstantBuffers(slot, 1, &constantBuffer);

	return slot;
}

void Shader::UpdateVSConstantBuffer(unsigned int slot, void* data) {
	try {
		m_dxo.context->UpdateSubresource(m_vsConstantBuffers.at(slot).Get(), 0, nullptr, data, 0, 0);
	} catch (const std::out_of_range& e) {
		__debugbreak();
		throw std::out_of_range("[Shader] UpdateVSConstantBuffer out of range slot !");
	}
}

void Shader::UpdatePSConstantBuffer(unsigned int slot, void* data) {
	try {
		m_dxo.context->UpdateSubresource(m_psConstantBuffers.at(slot).Get(), 0, nullptr, data, 0, 0);
	} catch (const std::out_of_range& e) {
		__debugbreak();
		throw std::out_of_range("[Shader] UpdatePSConstantBuffer out of range slot !");
	}
}
