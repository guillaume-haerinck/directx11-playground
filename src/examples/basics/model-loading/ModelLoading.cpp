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

		// Vertex shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(modelFactory.GetIed(), modelFactory.GetIedElementCount(), L"res/built-shaders/ModelLoading_VS.cso");
		comp::ConstantBuffer cameraCB = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(cb::Camera)));
		comp::ConstantBuffer meshVarCB = m_ctx.rcommand->CreateConstantBuffer(1, (sizeof(cb::MeshVariable) * 1));
		VShader.constantBuffers.push_back(cameraCB.buffer);
		VShader.constantBuffers.push_back(meshVarCB.buffer);

		// Pixel Shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/ModelLoading_PS.cso");
		
		// Set constant buffers to be updated in render system
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& constantBuffers = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
		constantBuffers.constantBuffers.at(scomp::ConstantBufferIndex::CAMERA) = cameraCB;
		constantBuffers.constantBuffers.at(scomp::ConstantBufferIndex::MESH_VARIABLES) = meshVarCB;

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
