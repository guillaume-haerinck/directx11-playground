#include "pch.h"
#include "RenderSystem.h"

#include "components/singletons/graphics/ConstantBuffers.h"
#include "components/singletons/graphics/Lights.h"
#include "components/singletons/graphics/Materials.h"
#include "components/physics/Transform.h"
#include "graphics/ConstantBuffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	m_timer.Tick([](){});
	auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// OPTIONAL ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// Update lights constant buffer
	{
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		if (lights.hasToBeUpdated) {
			// TODO
		}
	}

	// Update phong materials constant buffer
	{
		scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);
		if (materials.materials.size() > 0 && materials.hasToBeUpdated) {
			comp::ConstantBuffer& materialCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
				.constantBuffers.at(scomp::ConstantBufferIndex::PER_PHONG_MAT_CHANGE);

			cb::perPhongMaterialChange cbData = {}; // TODO is an array
			// TODO assert bytewidth of cb is same size of data

			m_ctx.rcommand->UpdateConstantBuffer(materialCB, &cbData);
			materials.hasToBeUpdated = false;
		}
	}

	// Update cook torrance constant buffer
	{
		scomp::CookTorranceMaterials& materials = m_ctx.registry.get<scomp::CookTorranceMaterials>(graphEntity);
		if (materials.materials.size() > 0 && materials.hasToBeUpdated) {
			comp::ConstantBuffer& materialCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
				.constantBuffers.at(scomp::ConstantBufferIndex::PER_COOK_MAT_CHANGE);

			cb::perCookTorranceMaterialChange cbData = {}; // TODO is an array
			// TODO assert bytewidth of cb is same size of data

			m_ctx.rcommand->UpdateConstantBuffer(materialCB, &cbData);
			materials.hasToBeUpdated = false;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NON-OPTIONAL //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// Update per frame constant buffer
	{
		cb::perFrame cbData = {};
		comp::ConstantBuffer& perFrameCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
			.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME);

		// TODO use camera component

		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 6.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);
		XMStoreFloat4x4(&cbData.matViewProj, view);

		m_ctx.rcommand->UpdateConstantBuffer(perFrameCB, &cbData);
	}

	// Render
	comp::ConstantBuffer& perMeshCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
		.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH);

	m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
		.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
		// Update perMesh constant buffer
		cb::perMesh cbData = {};
		XMStoreFloat4x4(&cbData.matModel, XMMatrixIdentity()); 	// TODO use transform component
		m_ctx.rcommand->UpdateConstantBuffer(perMeshCB, &cbData);
		
		// Bind
		m_ctx.rcommand->BindVertexShader(VShader);
		m_ctx.rcommand->BindPixelShader(PShader);
		m_ctx.rcommand->BindVertexBuffer(mesh.vb);
		m_ctx.rcommand->BindIndexBuffer(mesh.ib);
		if (mesh.textures.size() > 0) {
			m_ctx.rcommand->BindTextures(mesh.textures);
		}
		
		// Draw call
		m_ctx.rcommand->DrawIndexed(mesh.ib.count);
	});
}
