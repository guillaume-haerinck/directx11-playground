#include "pch.h"
#include "ModelLoading.h"

#include "factories/entities/ModelFactory.h"
#include "systems/RenderSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/singletons/graphics/ConstantBuffers.h"

namespace exemple {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		// Init
		ModelFactory modelFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Get constant buffers
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);

		// Vertex shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(modelFactory.GetIed(), modelFactory.GetIedElementCount(), L"res/built-shaders/ModelLoading_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);

		// Pixel Shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/ModelLoading_PS.cso");

		// Assign data to an entity
		auto entities = modelFactory.CreateEntitiesFromGltf("res/models/cube/Cube.gltf");
		m_ctx.registry.assign<comp::VertexShader>(entities.at(0), VShader);
		m_ctx.registry.assign<comp::PixelShader>(entities.at(0), PShader);
	}

	ModelLoading::~ModelLoading() {
	}

	void ModelLoading::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void ModelLoading::ImGuiUpdate() {
	}
}
