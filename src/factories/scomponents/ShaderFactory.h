#pragma once

#include "core/Context.h"

class ShaderFactory {
public:
	ShaderFactory(Context& context);
	~ShaderFactory();

	void SetIed(D3D11_INPUT_ELEMENT_DESC* iedArray, unsigned int iedElementCount);

	/**
	 * @brief
	 * @returns 
	 */
	unsigned int CreateVertexShader(LPCWSTR filePath, scomp::ConstantBufferIndex* cbIndexArray, unsigned int indexArrayElementCount);

	/**
	 * @brief
	 * @returns
	 */
	unsigned int CreatePixelShader(LPCWSTR filePath, scomp::ConstantBufferIndex* cbIndexArray, unsigned int indexArrayElementCount);

private:
	Context& m_ctx;
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_ied;
};
