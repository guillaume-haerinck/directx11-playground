#pragma once

#include "DXObjects.h"
#include "components/graphics/Mesh.h"
#include "scomponents/graphics/Shaders.h"
#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Materials.h"
#include "scomponents/graphics/Samplers.h"

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
	 * @param byteWidth - The total size in bytes of the buffer (must be a multiple of 16)
	 *
	 * @note - You have to store it within a shader component, and it will be bound with it.
	 *		   The slot will correspond to the index of the vector in the shader.
	 */
	scomp::ConstantBuffer CreateConstantBuffer(unsigned int byteWidth) const;

	/**
	 * @param slot - The texture slot the ressource will be bound to
	 */
	scomp::Sampler CreateSampler(scomp::SamplerSlot slot) const;

	/**
	 * @param slot - The texture slot the ressource will be bound to
	 * @param filepath - The relative path from the .exe to the image (.png or .jpg)
	 */
	scomp::Texture CreateTexture(unsigned int slot, LPCWSTR filepath, scomp::SamplerSlot samplerSlot = scomp::SamplerSlot::ANISOTROPIC_WRAP) const;

	/**
	 * @param iedArray - Input layout of the shader
	 * @param iedElementCount - Number of elements inside of the iedArray
	 * @param filepath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::VertexShader CreateVertexShader(D3D11_INPUT_ELEMENT_DESC* iedArray, unsigned int iedElementCount, LPCWSTR filePath) const;

	/**
	 * @param filePath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::PixelShader CreatePixelShader(LPCWSTR filePath) const;

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void BindVertexBuffer(comp::VertexBuffer vb) const;
	void BindIndexBuffer(comp::IndexBuffer ib) const;

	void BindSampler(scomp::Sampler sampler) const; // TODO BindSamplers
	void BindTextures(ID3D11ShaderResourceView* textures, unsigned int count) const;

	void BindVertexShader(scomp::VertexShader vs);
	void BindPixelShader(scomp::PixelShader ps);

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	void UpdateConstantBuffer(scomp::ConstantBuffer cb, void* data) const;

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
