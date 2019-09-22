#pragma once

#include "core/Context.h"

// TODO keep track of created shader ond check if it exist before creating it

class ShaderFactory {
public:
	ShaderFactory(Context& context);
	~ShaderFactory();

	void SetIed(D3D11_INPUT_ELEMENT_DESC* iedArray, unsigned int iedElementCount);

	/**
	 * @brief
	 * @returns 
	 */
	// TODO take a filepath and compile it if needed, so add flags as optionnal arg ?
	unsigned int CreateVertexShader(LPCWSTR filePath, scomp::ConstantBufferIndex* cbIndexArray = nullptr, unsigned int indexArrayElementCount = 0);

	/**
	 * @brief
	 * @returns
	 */
	unsigned int CreatePixelShader(LPCWSTR filePath, scomp::ConstantBufferIndex* cbIndexArray = nullptr, unsigned int indexArrayElementCount = 0);

	// TODO CompileAndCreateShader ? Or use file extension to know what to do ?

private:
	Context& m_ctx;
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_ied;
};
