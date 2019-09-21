#include "pch.h"
#include "ShaderFactory.h"

ShaderFactory::ShaderFactory(Context& context) : m_ctx(context) {
	m_ied = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}

ShaderFactory::~ShaderFactory()
{
}

void ShaderFactory::SetIed(D3D11_INPUT_ELEMENT_DESC* iedArray, unsigned int iedElementCount) {
	m_ied.resize(iedElementCount);
	for (int i = 0; i < iedElementCount; i++) {
		m_ied.at(i) = iedArray[i];
	}
}

unsigned int ShaderFactory::CreateVertexShader(LPCWSTR filePath, scomp::ConstantBufferIndex* cbIndexArray, unsigned int indexArrayElementCount) {
	// Get singleton components
	auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
	scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
	scomp::Shaders& shaders = m_ctx.registry.get<scomp::Shaders>(graphEntity);

	// Create shader
	scomp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(m_ied.data(), m_ied.size(), filePath);

	// Add constant buffers to shader
	VShader.constantBuffers.resize(indexArrayElementCount);
	for (int i = 0; i < indexArrayElementCount; i++) {
		VShader.constantBuffers.at(i) = cbs.constantBuffers.at(cbIndexArray[i]).buffer;
	}

	// Save shader
	shaders.vss.push_back(VShader);

	// Returns the index of the created shader
	return shaders.vss.size() - 1;
}

unsigned int ShaderFactory::CreatePixelShader(LPCWSTR filePath, scomp::ConstantBufferIndex* cbIndexArray, unsigned int indexArrayElementCount) {
	// Get singleton components
	auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
	scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
	scomp::Shaders& shaders = m_ctx.registry.get<scomp::Shaders>(graphEntity);

	// Create shader
	scomp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(filePath);

	// Add constant buffers to shader
	PShader.constantBuffers.resize(indexArrayElementCount);
	for (int i = 0; i < indexArrayElementCount; i++) {
		PShader.constantBuffers.at(i) = cbs.constantBuffers.at(cbIndexArray[i]).buffer;
	}

	// Save shader
	shaders.pss.push_back(PShader);

	// Returns the index of the created shader
	return shaders.pss.size() - 1;
}
