#include "pch.h"
#include "BasicTriangle.h"

#include "graphics/DXException.h"
#include "factories/scomponents/ShaderFactory.h"
#include "components/graphics/Mesh.h"
#include "components/graphics/Pipeline.h"
#include "components/physics/Transform.h"
#include "scomponents/graphics/Shaders.h"
#include "systems/RenderSystem.h"

namespace basicExample {
	BasicTriangle::BasicTriangle(Context& context) : m_ctx(context) {
		// Init
		ShaderFactory shaderFactory(context);
		m_systems = {
			std::make_shared<RenderSystem>(context)
		};

		// Position attribute buffer
		DX::XMFLOAT2 positions[] = {
			DX::XMFLOAT2(0.0f,  0.5f),
			DX::XMFLOAT2(0.5f, -0.5f),
			DX::XMFLOAT2(-0.5f, -0.5f)
		};
		comp::AttributeBuffer positionBuffer = m_ctx.rcommand->CreateAttributeBuffer(positions, sizeof(positions), sizeof(DX::XMFLOAT2));

		// Vertex buffer
		comp::VertexBuffer vb = {};
		vb.buffers = { positionBuffer.buffer };
		vb.byteWidths = { positionBuffer.byteWidth };
		vb.counts = { positionBuffer.count };
		vb.strides = { positionBuffer.stride };
		vb.offsets = { 0 };
		vb.names = { "position" };

		// Index buffer
		WORD indices[] = { 0, 1, 2 };
		comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer(indices, ARRAYSIZE(indices));
		
		// Mesh
		comp::Mesh mesh = {};
		mesh.vb = vb;
		mesh.ib = ib;

		// Vertex shader
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		shaderFactory.SetIed(ied, ARRAYSIZE(ied));
		unsigned int vsID = shaderFactory.CreateVertexShader(L"res/built-shaders/BasicTriangle_VS.cso");

		// Pixel Shader
		unsigned int psID = shaderFactory.CreatePixelShader(L"res/built-shaders/BasicTriangle_PS.cso");

		// Pipeline
		comp::Pipeline pipeline = {};
		pipeline.psIndex = psID;
		pipeline.vsIndex = vsID;

		// Transform
		comp::Transform transform = {};

		// Create entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	BasicTriangle::~BasicTriangle() {
	}

	void BasicTriangle::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void BasicTriangle::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Hello triangle !");
		ImGui::Text("Ready ?");
		ImGui::End();
	}
}
