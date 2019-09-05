#pragma once

#include "DXObjects.h"
#include "components/graphics/Mesh.h"
#include "components/graphics/Shader.h"
#include "components/graphics/Material.h"
#include "components/singletons/graphics/Sampler.h"

// TODO keep track of already created shaders

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

	/**
	 * @param vertices - Array of data
	 * @param count - The number of elements in the array
	 * @param stride - Size in bytes of one element of the array
	 */
	comp::AttributeBuffer CreateAttributeBuffer(void* vertices, unsigned int count, unsigned int stride) const;

	/**
	 * @param indices - Array of data
	 * @param count - The number of elements in the array
	 */
	comp::IndexBuffer CreateIndexBuffer(void* indices, unsigned int count) const;

	/**
	 * @param slot - The register in the shader that the buffer will be bound to (c0 or c1; etc)
	 * @param byteWidth - The total size in bytes of the buffer
	 */
	comp::ConstantBuffer CreateConstantBuffer(unsigned int slot, unsigned int byteWidth) const;

	/**
	 * @param slot - The texture slot the ressource will be bound to
	 */
	comp::Sampler CreateSampler(comp::SamplerSlot slot) const;

	/**
	 * @param slot - The texture slot the ressource will be bound to
	 * @param filepath - The relative path from the .exe to the image (.png or .jpg)
	 */
	comp::Texture CreateTexture(unsigned int slot, LPCWSTR filepath, comp::SamplerSlot samplerSlot = comp::SamplerSlot::ANISOTROPIC_WRAP) const;

	/**
	 * @param iedArray - Input layout of the shader
	 * @param iedElementCount - Number of elements inside of the iedArray
	 * @param filepath - The relative path from the .exe to the .cso containing the shader
	 */
	comp::VertexShader CreateVertexShader(D3D11_INPUT_ELEMENT_DESC* iedArray, unsigned int iedElementCount, LPCWSTR filePath) const;

	/**
	 * @param filePath - The relative path from the .exe to the .cso containing the shader
	 */
	comp::PixelShader CreatePixelShader(LPCWSTR filePath) const;

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void BindVertexBuffer(comp::VertexBuffer vb) const;
	void BindIndexBuffer(comp::IndexBuffer ib) const;

	void BindSampler(comp::Sampler sampler) const;
	void BindTexture(comp::Texture texture) const;

	// TODO BindTextures

	void BindVertexShader(comp::VertexShader vs);
	void BindPixelShader(comp::PixelShader ps);

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	void UpdateConstantBuffer(comp::ConstantBuffer cb, void* data) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void Draw(unsigned int count) const;
	void DrawIndexed(unsigned int count) const;
	
private:
	DXObjects& m_dxo;
	
	ID3D11VertexShader* m_lastVShaderBound;
	ID3D11PixelShader* m_lastPShaderBound;
};
