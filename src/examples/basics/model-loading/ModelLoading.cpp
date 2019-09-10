#include "pch.h"
#include "ModelLoading.h"

#include "factories/entities/ModelFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/physics/Transform.h"
#include "components/singletons/graphics/ConstantBuffers.h"

namespace basicExample {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		// Init
		ModelFactory modelFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

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

		// Assign data to entities
		auto entities = modelFactory.CreateEntitiesFromGltf("res/models/damaged-helmet/DamagedHelmet.gltf");
		for (auto entity : entities) {
			m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
			m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
			m_ctx.registry.assign<comp::Transform>(entity, transform);
		}
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
