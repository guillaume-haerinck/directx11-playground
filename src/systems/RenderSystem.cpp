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

	// Update phong materials constant buffer
	{
		scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);
		if (materials.materials.size() > 0 && materials.hasToBeUpdated) {
			comp::ConstantBuffer& materialCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
				.constantBuffers.at(scomp::ConstantBufferIndex::PHONG_MATERIALS);

			cb::PhongMaterial materialData = {}; // TODO is an array
			// TODO assert bytewidth of cb is same size of data

			m_ctx.rcommand->UpdateConstantBuffer(materialCB, &materialData);
			materials.hasToBeUpdated = false;
		}
	}

	// Update cook torrance constant buffer
	{
		scomp::CookTorranceMaterials& materials = m_ctx.registry.get<scomp::CookTorranceMaterials>(graphEntity);
		if (materials.materials.size() > 0 && materials.hasToBeUpdated) {
			comp::ConstantBuffer& materialCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
				.constantBuffers.at(scomp::ConstantBufferIndex::COOK_TORRANCE_MATERIALS);

			cb::CookTorranceMaterial materialData = {}; // TODO is an array
			// TODO assert bytewidth of cb is same size of data

			m_ctx.rcommand->UpdateConstantBuffer(materialCB, &materialData);
			materials.hasToBeUpdated = false;
		}
	}

	// Render
	m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
		.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
		m_ctx.rcommand->BindVertexShader(VShader);
		m_ctx.rcommand->BindPixelShader(PShader);
		m_ctx.rcommand->BindVertexBuffer(mesh.vb);
		m_ctx.rcommand->BindIndexBuffer(mesh.ib);
		if (mesh.textures.size() > 0) {
			m_ctx.rcommand->BindTextures(mesh.textures);
		}
		m_ctx.rcommand->DrawIndexed(mesh.ib.count);
	});
}
