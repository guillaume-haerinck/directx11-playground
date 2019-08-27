#pragma once

#include "DXObjects.h"

/**
 * @brief Issue draw commands to Direct3D API
 */
class RenderCommand {
public:
	RenderCommand(DXObjects& dxObjects);

	/**
	 * @brief Clear the back buffer
	 */
	void Clear() const;

	/**
	 * @brief Swap front and back buffers
	 */
	void Swap() const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// CREATION ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @param vertices - Array of data
	 * @param byteWidth - Total size of the array in bytes
	 * @param stride - Size in bytes of one element of the array
	 */
	ID3D11Buffer* CreateVertexBuffer(void* vertices, unsigned int byteWidth, unsigned int stride) const;
	ID3D11Buffer* CreateIndexBuffer(WORD* indices, unsigned int count) const;
	ID3D11Buffer* CreateConstantBuffer(unsigned int byteWidth) const;

	std::tuple<ID3D11VertexShader*, ID3D11InputLayout*> CreateVertexShader(D3D11_INPUT_ELEMENT_DESC* ied, unsigned int iedElementCount, LPCWSTR filePath) const;
	ID3D11PixelShader* CreatePixelShader(LPCWSTR filePath) const;

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void BindVertexBuffer(ID3D11Buffer* buffer, unsigned int stride) const;
	void BindIndexBuffer(ID3D11Buffer* buffer) const;

	void BindVertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* layout) const;
	void BindVSConstantBuffer(ID3D11Buffer* buffer, unsigned int slot) const;

	void BindPixelShader(ID3D11PixelShader* shader) const;
	void BindPSConstantBuffer(ID3D11Buffer* buffer, unsigned int slot) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	void UpdateConstantBuffer(ID3D11Buffer* buffer, void* data, unsigned int size) const;
	
private:
	DXObjects& m_dxo;
};
