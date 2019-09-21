#include "pch.h"
#include "PhongDirectionalLight.h"

#include "graphics/ConstantBuffer.h"
#include "factories/entities/ModelFactory.h"
#include "factories/scomponents/ShaderFactory.h"
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
		ShaderFactory shaderFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Get constant buffers
		auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);

		// Light
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		lights.hasToBeUpdated = true;

		scomp::DirectionalLight light0 = {};
		light0.color = DX::XMFLOAT3(1, 1, 1);
		light0.intensity = 1.0f;
		light0.direction = DX::XMFLOAT3(1, -1, 1);
		lights.directionalLights.push_back(light0);

		// Init exemple specific constant buffers
		scomp::ConstantBuffer perLightCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::perLightChange) * 1);
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE) = perLightCB;
		m_perPropertyCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(perPropertyChange));
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_CUSTOM_PROP_CHANGE) = m_perPropertyCB;

		// Update custom constant buffer
		m_perPropertyCBdata.ambientIntensity = 1.0f;
		m_perPropertyCBdata.diffuseIntensity = 1.0f;
		m_perPropertyCBdata.specularIntensity = 1.0f;
		m_perPropertyCBdata.specularAttenuation = 20.0f;
		m_ctx.rcommand->UpdateConstantBuffer(m_perPropertyCB, &m_perPropertyCBdata);

		// Vertex shader
		shaderFactory.SetIed(modelFactory.GetIed(), modelFactory.GetIedElementCount());
		scomp::ConstantBufferIndex vsCbArray[] = {
			scomp::ConstantBufferIndex::PER_MESH,
			scomp::ConstantBufferIndex::PER_FRAME
		};
		unsigned int vsID = shaderFactory.CreateVertexShader(L"res/built-shaders/PhongDirectionalLight_VS.cso", vsCbArray, ARRAYSIZE(vsCbArray));

		// Pixel Shader
		scomp::ConstantBufferIndex psCbArray[] = {
			scomp::ConstantBufferIndex::PER_LIGHT_CHANGE,
			scomp::ConstantBufferIndex::PER_CUSTOM_PROP_CHANGE,
		};
		unsigned int psID = shaderFactory.CreatePixelShader(L"res/built-shaders/PhongDirectionalLight_PS.cso", psCbArray, ARRAYSIZE(psCbArray));

		comp::Pipeline pipeline = {};
		pipeline.hasShader.at(comp::PipelineShaderIndex::PS) = true;
		pipeline.hasShader.at(comp::PipelineShaderIndex::VS) = true;
		pipeline.psIndex = psID;
		pipeline.vsIndex = vsID;

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
