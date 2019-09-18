#include "pch.h"
#include "RenderSystem.h"

#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Lights.h"
#include "scomponents/graphics/Materials.h"
#include "scomponents/graphics/Camera.h"
#include "components/physics/Transform.h"
#include "graphics/ConstantBuffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// OPTIONAL ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// Update lights constant buffer
	{
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		if (lights.hasToBeUpdated) {
			comp::ConstantBuffer& lightCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
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

	// Render
	comp::ConstantBuffer& perMeshCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
		.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH);

	m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader, comp::Transform>()
		.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader, comp::Transform& transform) {
		// Update perMesh constant buffer
		cb::perMesh cbData = {};
		DX::XMVECTOR transVector = DX::XMLoadFloat3(&transform.position);
		DX::XMVECTOR scaleVector = DX::XMLoadFloat3(&transform.scale);
		DX::XMVECTOR rotationQuat = DX::XMLoadFloat4(&transform.rotation);

		DX::XMMATRIX model = DX::XMMatrixAffineTransformation(scaleVector, DX::XMVectorZero(), rotationQuat, transVector);
		DX::XMStoreFloat4x4(&cbData.matModel, DX::XMMatrixTranspose(model));
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
