#pragma once

#include "DXObjects.h"
#include "components/graphics/Mesh.h"
#include "components/graphics/Shader.h"

// TODO keep track of bound objects with static data
// TODO keep track of already created shaders
// TODO remove access to DXObjects to every class so that they must use rendercommand
// TODO make the whole class static, search for best performance
// TODO check performance problems when nesting in a Context struct

/**
 * @brief Issue draw commands to Direct3D API
 */
class RenderCommand {
public:
	RenderCommand(DXObjects& dxObjects);

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////// BACK BUFFER ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////

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

	// TODO return a component for each

	/**
	 * @param vertices - Array of data
	 * @param count - The number of elements in the array
	 * @param stride - Size in bytes of one element of the array
	 */
	comp::VertexBuffer CreateVertexBuffer(void* vertices, unsigned int count, unsigned int stride) const;
	comp::IndexBuffer CreateIndexBuffer(WORD* indices, unsigned int count) const;
	comp::ConstantBuffer CreateConstantBuffer(unsigned int slot, unsigned int byteWidth) const;

	std::tuple<ID3D11VertexShader*, ID3D11InputLayout*> CreateVertexShader(D3D11_INPUT_ELEMENT_DESC* ied, unsigned int iedElementCount, LPCWSTR filePath) const;
	ID3D11PixelShader* CreatePixelShader(LPCWSTR filePath) const;

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void BindVertexBuffer(comp::VertexBuffer vb) const;
	void BindIndexBuffer(comp::IndexBuffer ib) const;

	void BindVertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* layout);
	void BindVSConstantBuffer(comp::ConstantBuffer cb) const;

	void BindPixelShader(ID3D11PixelShader* shader);
	void BindPSConstantBuffer(comp::ConstantBuffer cb) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	void UpdateConstantBuffer(comp::ConstantBuffer cb, void* data) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING ////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	void Draw(unsigned int count) const;
	void DrawIndexed(unsigned int count) const;
	
private:
	DXObjects& m_dxo;
	
	static ID3D11VertexShader* m_lastVShaderBound;
	static ID3D11PixelShader* m_lastPShaderBound;
};
