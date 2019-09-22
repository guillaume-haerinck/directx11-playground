#include "pch.h"
#include "ModelLoading.h"

#include "factories/entities/ModelFactory.h"
#include "factories/scomponents/ShaderFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/graphics/Pipeline.h"
#include "components/physics/Transform.h"
#include "scomponents/graphics/ConstantBuffers.h"

namespace basicExample {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		// Init
		ModelFactory modelFactory(context);
		ShaderFactory shaderFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Vertex shader
		shaderFactory.SetIed(modelFactory.GetIed(), modelFactory.GetIedElementCount());
		scomp::ConstantBufferIndex vsCbArray[] = {
			scomp::ConstantBufferIndex::PER_MESH,
			scomp::ConstantBufferIndex::PER_FRAME
		};
		unsigned int vsID = shaderFactory.CreateVertexShader(L"res/built-shaders/ModelLoading_VS.cso", vsCbArray, ARRAYSIZE(vsCbArray));

		// Pixel Shader
		unsigned int psID = shaderFactory.CreatePixelShader(L"res/built-shaders/ModelLoading_PS.cso");

		// Setup pipeline
		comp::Pipeline pipeline = {};
		pipeline.psIndex = psID;
		pipeline.vsIndex = vsID;

		// Transform
		comp::Transform transform = {};

		// Assign data to entities
		auto entities = modelFactory.CreateEntitiesFromGltf("res/models/damaged-helmet/DamagedHelmet.gltf");
		for (auto entity : entities) {
			m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
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
