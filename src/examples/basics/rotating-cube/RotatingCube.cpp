#include "pch.h"
#include "RotatingCube.h"

#include "factories/components/MeshPrimitiveFactory.h"
#include "components/graphics/Mesh.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/physics/Transform.h"
#include "scomponents/graphics/ConstantBuffers.h"

namespace basicExample {
	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Get constant buffers
		auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);

		// Vertex Shader
		scomp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/RotatingCube_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);

		// Pixel shader
		scomp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/RotatingCube_PS.cso");
		scomp::ConstantBuffer colorCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(DX::XMFLOAT4) * 6);
		PShader.constantBuffers.push_back(colorCB.buffer);

		// Update PSconstant buffer once as it will not change
		DX::XMFLOAT4 colorCBdata[6] = {
			DX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			DX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			DX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			DX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			DX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_ctx.rcommand->UpdateConstantBuffer(colorCB, &colorCBdata);

		// Transformm
		comp::Transform transform = {};

		// Save data to entity
		auto entity = m_ctx.registry.create();
		comp::Mesh mesh = primFactory.CreateBox();
		m_ctx.registry.assign<scomp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<scomp::PixelShader>(entity, PShader);
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
