#include "pch.h"
#include "ModelLoading.h"

#include "factories/entities/ModelFactory.h"
#include "systems/RenderSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/physics/Transform.h"
#include "components/singletons/graphics/ConstantBuffers.h"

namespace basicExample {
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

		// Transformm
		comp::Transform transform = {};

		// Assign data to an entity
		auto entities = modelFactory.CreateEntitiesFromGltf("res/models/damaged-helmet/DamagedHelmet.gltf");
		m_ctx.registry.assign<comp::VertexShader>(entities.at(0), VShader);
		m_ctx.registry.assign<comp::PixelShader>(entities.at(0), PShader);
		m_ctx.registry.assign<comp::Transform>(entities.at(0), transform);
	}

	ModelLoading::~ModelLoading() {
	}

	void ModelLoading::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void ModelLoading::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Hello model !");
		ImGui::End();
	}
}
