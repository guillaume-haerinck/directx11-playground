#include "pch.h"
#include "RenderSystem.h"

#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Lights.h"
#include "scomponents/graphics/Materials.h"
#include "scomponents/graphics/Camera.h"
#include "components/graphics/Pipeline.h"
#include "components/physics/Transform.h"
#include "graphics/ConstantBuffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// OPTIONAL ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// Update lights constant buffer
	{
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		if (lights.hasToBeUpdated) {
			scomp::ConstantBuffer& lightCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
				.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE);

			std::vector<cb::perLightChange> cbData;
			cbData.resize(
				lights.spotLights.size() +
				lights.pointLights.size() +
				lights.directionalLights.size() 
			);

			int i = 0;
			for (auto& light : lights.spotLights) {
				cbData.at(i).color = light.color;
				cbData.at(i).spotAngle = light.spotAngle;
				cbData.at(i).position = light.position;
				cbData.at(i).intensity = light.intensity;
				cbData.at(i).direction = light.direction;
				cbData.at(i).attenuationRadius = light.attenuationRadius;
				i++;
			}

			for (auto& light : lights.pointLights) {
				cbData.at(i).color = light.color;
				cbData.at(i).intensity = light.intensity;
				cbData.at(i).position = light.position;
				cbData.at(i).attenuationRadius = light.attenuationRadius;
				i++;
			}

			for (auto& light : lights.directionalLights) {
				cbData.at(i).color = light.color;
				cbData.at(i).intensity = light.intensity;
				cbData.at(i).direction = light.direction;
				i++;
			}

			assert(sizeof(cb::perLightChange) * cbData.size() == lightCB.byteWidth && "The allocated space for the light constant buffer does not match the lights declared ! Check CreateConstantBuffer()");

			m_ctx.rcommand->UpdateConstantBuffer(lightCB, cbData.data());
			lights.hasToBeUpdated = false;
		}
	}

	// Update phong materials constant buffer
	{
		scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);
		if (materials.materials.size() > 0 && materials.hasToBeUpdated) {
			scomp::ConstantBuffer& materialCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
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
			scomp::ConstantBuffer& materialCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
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
		scomp::ConstantBuffer& perFrameCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
			.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME);

		// Get usable data
		scomp::Camera camera = m_ctx.registry.get<scomp::Camera>(graphEntity);
		DX::XMMATRIX view = DX::XMLoadFloat4x4(&camera.view);
		DX::XMMATRIX proj = DX::XMLoadFloat4x4(&camera.proj);

		// Update data. Use column-major matrix for HLSL
		cbData.cameraPos = camera.position;
		DX::XMMATRIX viewProj = DX::XMMatrixTranspose(view * proj);
		DX::XMStoreFloat4x4(&cbData.matViewProj, viewProj);

		// Send data
		m_ctx.rcommand->UpdateConstantBuffer(perFrameCB, &cbData);
	}

	// Get singleton components used for rendering
	scomp::Shaders& shaders = m_ctx.registry.get<scomp::Shaders>(graphEntity);
	scomp::ConstantBuffer& perMeshCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
		.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH);

	// Render
	m_ctx.registry.view<comp::Mesh, comp::Pipeline, comp::Transform>()
		.each([&](comp::Mesh& mesh, comp::Pipeline& pipeline, comp::Transform& transform) {
		// Update perMesh constant buffer
		cb::perMesh cbData = {};
		DX::XMVECTOR transVector = DX::XMLoadFloat3(&transform.position);
		DX::XMVECTOR scaleVector = DX::XMVectorSet(transform.scale, transform.scale, transform.scale, 1);
		DX::XMVECTOR rotationQuat = DX::XMLoadFloat4(&transform.rotation);

		DX::XMMATRIX model = DX::XMMatrixAffineTransformation(scaleVector, DX::XMVectorZero(), rotationQuat, transVector);
		DX::XMStoreFloat4x4(&cbData.matModel, DX::XMMatrixTranspose(model));
		m_ctx.rcommand->UpdateConstantBuffer(perMeshCB, &cbData);
		
		// TODO handle pipeline to check if has said shader
		// TODO trycatch to check if .at exist

		// Bind
		m_ctx.rcommand->BindVertexShader(shaders.vss.at(pipeline.vsIndex));
		m_ctx.rcommand->BindPixelShader(shaders.pss.at(pipeline.psIndex));
		m_ctx.rcommand->BindVertexBuffer(mesh.vb);
		m_ctx.rcommand->BindIndexBuffer(mesh.ib);

		// Bind textures
		switch (mesh.materialType) {
		case scomp::MaterialType::PHONG: {
			scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);
			scomp::PhongMaterial material = materials.materials.at(mesh.materialIndex);
			// TODO how to bind them all at once when one might be missing ? Use dumb data ?
			for (auto texture : material.textures) {
				m_ctx.rcommand->BindTextures(texture.srv.Get(), 1);
			}
			break;
		}

		default:
			break;
		}
		
		// Draw call
		m_ctx.rcommand->DrawIndexed(mesh.ib.count);
	});
}
