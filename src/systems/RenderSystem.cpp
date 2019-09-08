#include "pch.h"
#include "RenderSystem.h"

#include "components/singletons/graphics/ConstantBuffers.h"
#include "graphics/ConstantBuffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	m_timer.Tick([](){});
	auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);

	// Update camera constant buffer
	{
		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 6.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);

		cb::Camera cameraData = {};
		XMStoreFloat4x4(&cameraData.matViewProj, view);

		comp::ConstantBuffer& cameraCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
			.constantBuffers.at(scomp::ConstantBufferIndex::CAMERA);
		m_ctx.rcommand->UpdateConstantBuffer(cameraCB, &cameraData);
	}

	// Update mesh variable constant buffer
	{
		// TODO is an array, size corresponding to the maximum number of mesh in the scene
		// OR update for each object between each draw call
		cb::MeshVariable meshVarData = {};
		// meshVarData.materialIndex = 0;
		XMStoreFloat4x4(&meshVarData.matModel, XMMatrixIdentity());

		comp::ConstantBuffer& meshVarCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
			.constantBuffers.at(scomp::ConstantBufferIndex::MESH_VARIABLES);
		m_ctx.rcommand->UpdateConstantBuffer(meshVarCB, &meshVarData);
	}

	// Update lights constant buffer
	{
		// TODO
	}

	// Update materials constant buffer
	{
		// TODO
	}

	// Render
	m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
		.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
		m_ctx.rcommand->BindVertexShader(VShader);
		m_ctx.rcommand->BindPixelShader(PShader);
		m_ctx.rcommand->BindVertexBuffer(mesh.vb);
		m_ctx.rcommand->BindIndexBuffer(mesh.ib);
		m_ctx.rcommand->DrawIndexed(mesh.ib.count);
	});
}
