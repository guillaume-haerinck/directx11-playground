#pragma once

#include "DXObjects.h"

// TODO create a class for geometry shader
// TODO create a class for tesselation stage

/**
 * @brief Merge and abstract Vertex shader and Pixel shader operations
 */
class Shader {
public:
	// TODO create hashmap from inputBuffer to reuse it if already exist

	/**
	 * @brief Create the vertex shader, the input assembler and the vertex shader to DirectX
	 *
	 * @param dxObjects - 
	 * @param ied -
	 * @param iedElementCount -
	 * @param vsFilePath -
	 * @param psFilePath -
	 */
	Shader(DXObjects dxObjects, D3D11_INPUT_ELEMENT_DESC* ied, unsigned int iedElementCount, LPCWSTR vsFilePath, LPCWSTR psFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	/**
	 * @brief Add a new constant buffer to the Vertex Shader
	 *
	 * @param byteWidth - Size of the buffer in bytes
	 * @returns The assigned slot
	 */
	unsigned int AddVSConstantBuffer(unsigned int byteWidth);
	unsigned int AddPSConstantBuffer(unsigned int byteWidth);

	/**
	 * @brief Update the Vertex Shader constant buffer at the specified slot
	 * @note Must be called once per frame
	 */
	void UpdateVSConstantBuffer(unsigned int slot, void* data);
	void UpdatePSConstantBuffer(unsigned int slot, void* data);

private:
	DXObjects m_dxo;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_vsConstantBuffers;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_psConstantBuffers;
};
