#include "pch.h"
#include "ModelLoading.h"

#include "factories/entities/ModelFactory.h"
#include "systems/RenderSystem.h"
#include "graphics/ConstantBuffers.h"
#include "components/singletons/graphics/Camera.h"

namespace exemple {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		ModelFactory modelFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(modelFactory.GetIed(), modelFactory.GetIedElementCount(), L"res/built-shaders/ModelLoading_VS.cso");
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/ModelLoading_PS.cso");
		auto VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(cb::TEMP)));
		VShader.constantBuffers.push_back(VSCB0);

		// TODO find a more global and safer way to init singleton components (use POO instead ?)
		// Init camera constant buffer
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::Camera camera = {};
		camera.constantBuffer = VSCB0;
		m_ctx.registry.assign<scomp::Camera>(graphEntity, camera);

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
