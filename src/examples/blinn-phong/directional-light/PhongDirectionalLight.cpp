#include "pch.h"
#include "PhongDirectionalLight.h"

#include "graphics/ConstantBuffer.h"
#include "factories/entities/ModelFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "components/physics/Transform.h"
#include "components/graphics/Pipeline.h"
#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Lights.h"

namespace phongExample {
	PhongDirectionalLight::PhongDirectionalLight(Context& context) : m_ctx(context) {
		// Init
		ModelFactory modelFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Get constant buffers
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);

		// Light
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		lights.hasToBeUpdated = true;

		scomp::DirectionalLight light0 = {};
		light0.color = DX::XMFLOAT3(1, 1, 1);
		light0.intensity = 1.0f;
		light0.direction = DX::XMFLOAT3(1, -1, 1);
		lights.directionalLights.push_back(light0);

		// Init non-optionnal constant buffer
		scomp::ConstantBuffer perLightCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::perLightChange) * 1);
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE) = perLightCB;

		// Vertex shader
		scomp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(modelFactory.GetIed(), modelFactory.GetIedElementCount(), L"res/built-shaders/PhongDirectionalLight_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);

		// Pixel Shader
		scomp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/PhongDirectionalLight_PS.cso");
		PShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE).buffer);

		// Pixel shader custom constant buffer for this example
		m_perPropertyCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(perPropertyChange));
		PShader.constantBuffers.push_back(m_perPropertyCB.buffer);
		m_perPropertyCBdata.ambientIntensity = 1.0f;
		m_perPropertyCBdata.diffuseIntensity = 1.0f;
		m_perPropertyCBdata.specularIntensity = 1.0f;
		m_perPropertyCBdata.specularAttenuation = 20.0f;
		m_ctx.rcommand->UpdateConstantBuffer(m_perPropertyCB, &m_perPropertyCBdata);

		// Setup pipeline
		scomp::Shaders& shaders = m_ctx.registry.get<scomp::Shaders>(graphEntity);
		shaders.pss.push_back(PShader);
		shaders.vss.push_back(VShader);

		comp::Pipeline pipeline = {};
		pipeline.hasShader.at(comp::PipelineShaderIndex::PS) = true;
		pipeline.hasShader.at(comp::PipelineShaderIndex::VS) = true;
		pipeline.psIndex = shaders.pss.size() - 1;
		pipeline.vsIndex = shaders.vss.size() - 1;

		// Transform
		comp::Transform transform = {};

		// Assign data to an entity
		auto entities = modelFactory.CreateEntitiesFromGltf("res/models/damaged-helmet/DamagedHelmet.gltf");
		m_litEntity = entities.at(0);
		for (auto entity : entities) {
			m_ctx.registry.assign<comp::Transform>(entity, transform);
			m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		}
	}

	PhongDirectionalLight::~PhongDirectionalLight()
	{
	}

	void PhongDirectionalLight::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void PhongDirectionalLight::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");

		comp::Transform& transform = m_ctx.registry.get<comp::Transform>(m_litEntity);
		ImGui::Text("Object :");
		ImGui::SliderFloat3("Rotation", (float*) &transform.rotationEuler, 0, DX::XM_PI * 2);
		DX::XMStoreFloat4(&transform.rotation, DX::XMQuaternionRotationRollPitchYawFromVector(DX::XMLoadFloat3(&transform.rotationEuler)));

		ImGui::Spacing();

		bool hasToBeUpdated = false;
		ImGui::Text("Light :");
		hasToBeUpdated |= ImGui::SliderFloat("Ambient intensity", &m_perPropertyCBdata.ambientIntensity, 0, 10);
		hasToBeUpdated |= ImGui::SliderFloat("Diffuse intensity", &m_perPropertyCBdata.diffuseIntensity, 0, 10);
		hasToBeUpdated |= ImGui::SliderFloat("Specular intensity", &m_perPropertyCBdata.specularIntensity, 0, 10);
		hasToBeUpdated |= ImGui::SliderFloat("Specular attenuation", &m_perPropertyCBdata.specularAttenuation, 1, 45);
		if (hasToBeUpdated) {
			m_ctx.rcommand->UpdateConstantBuffer(m_perPropertyCB, &m_perPropertyCBdata);
		}

		ImGui::End();
	}
}
