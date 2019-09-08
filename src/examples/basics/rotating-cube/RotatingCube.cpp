#include "pch.h"
#include "RotatingCube.h"

#include "factories/components/MeshPrimitiveFactory.h"
#include "components/graphics/Mesh.h"
#include "systems/RenderSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/singletons/graphics/ConstantBuffers.h"

namespace exemple {
	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Vertex Shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/RotatingCube_VS.cso");
		comp::ConstantBuffer cameraCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::Camera));
		comp::ConstantBuffer meshVarCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::MeshVariable) * 1);
		VShader.constantBuffers.push_back(cameraCB.buffer);
		VShader.constantBuffers.push_back(meshVarCB.buffer);

		// Pixel shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/RotatingCube_PS.cso");
		comp::ConstantBuffer colorCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(XMFLOAT4) * 6);
		PShader.constantBuffers.push_back(colorCB.buffer);

		// Update PSconstant buffer once as it will not change
		XMFLOAT4 colorCBdata[6] = {
			XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_ctx.rcommand->UpdateConstantBuffer(colorCB, &colorCBdata);

		// Set constant buffers to be updated in render system
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& constantBuffers = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
		constantBuffers.constantBuffers.at(scomp::ConstantBufferIndex::CAMERA) = cameraCB;
		constantBuffers.constantBuffers.at(scomp::ConstantBufferIndex::MESH_VARIABLES) = meshVarCB;

		// Save data to entity
		auto entity = m_ctx.registry.create();
		comp::Mesh mesh = primFactory.CreateBox();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
	}

	RotatingCube::~RotatingCube() {
	}

	void RotatingCube::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
