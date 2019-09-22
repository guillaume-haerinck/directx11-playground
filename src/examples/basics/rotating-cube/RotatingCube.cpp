#include "pch.h"
#include "RotatingCube.h"

#include "factories/components/MeshPrimitiveFactory.h"
#include "factories/scomponents/ShaderFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "components/physics/Transform.h"
#include "components/graphics/Mesh.h"
#include "components/graphics/Pipeline.h"
#include "components/graphics/Layer.h"
#include "scomponents/graphics/ConstantBuffers.h"

namespace basicExample {
	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		ShaderFactory shaderFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Vertex shader
		shaderFactory.SetIed(primFactory.GetIed(), primFactory.GetIedElementCount());
		scomp::ConstantBufferIndex vsCbArray[] = {
			scomp::ConstantBufferIndex::PER_MESH,
			scomp::ConstantBufferIndex::PER_FRAME
		};
		unsigned int vsID = shaderFactory.CreateVertexShader(L"res/built-shaders/RotatingCube_VS.cso", vsCbArray, ARRAYSIZE(vsCbArray));

		// Custom constant buffer
		auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
		scomp::ConstantBuffer colorCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(DX::XMFLOAT4) * 6);
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_CUSTOM_PROP_CHANGE_0) = colorCB;

		// Pixel Shader
		scomp::ConstantBufferIndex psCbArray[] = {
			scomp::ConstantBufferIndex::PER_CUSTOM_PROP_CHANGE_0,
		};
		unsigned int psID = shaderFactory.CreatePixelShader(L"res/built-shaders/RotatingCube_PS.cso", psCbArray, ARRAYSIZE(psCbArray));

		// Update custom constant buffer
		DX::XMFLOAT4 colorCBdata[6] = {
			DX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			DX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			DX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			DX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			DX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_ctx.rcommand->UpdateConstantBuffer(colorCB, &colorCBdata);

		// Setup pipeline
		comp::Pipeline pipeline = {};
		pipeline.psIndex = psID;
		pipeline.vsIndex = vsID;

		// Transform
		comp::Transform transform = {};

		// Assign data to entity
		auto entity = m_ctx.registry.create();
		comp::Mesh mesh = primFactory.CreateBox();
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	RotatingCube::~RotatingCube() {
	}

	void RotatingCube::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void RotatingCube::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Hello cube !");
		ImGui::End();
	}
}
