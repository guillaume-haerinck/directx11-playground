#include "pch.h"
#include "CameraSystem.h"

#include "components/singletons/io/Inputs.h"
#include "components/singletons/graphics/Camera.h"

CameraSystem::CameraSystem(Context& context) : m_ctx(context)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Update() {
	// Get singleton components
	auto ioEntity = m_ctx.singletonComponents.at(SingletonComponents::IO);
	scomp::Inputs inputs = m_ctx.registry.get<scomp::Inputs>(ioEntity);
	auto graphicEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
	scomp::Camera& camera = m_ctx.registry.get<scomp::Camera>(graphicEntity);

	// TODO ArcBall rotation
	if (inputs.actionState.at(scomp::InputAction::CAM_ORBIT)) {
		camera.position.x -= inputs.delta.x * 0.03;
		camera.position.y -= inputs.delta.y * 0.03;

		XMVECTOR eye = XMLoadFloat3(&camera.position);
		XMVECTOR target = XMLoadFloat3(&camera.target);
		XMVECTOR up = XMVectorSet(0, 1, 0, 1);

		XMMATRIX view = XMMatrixLookAtLH(-eye, target, up);
		XMStoreFloat4x4(&camera.view, view);
	}

	// Move along vertical plane
	if (inputs.actionState.at(scomp::InputAction::CAM_PAN)) {
		camera.position.x -= inputs.delta.x * 0.005;
		camera.position.y += inputs.delta.y * 0.005;

		// TODO move target ?

		XMVECTOR translation = XMLoadFloat3(&camera.position);
		XMMATRIX view = XMMatrixTranslationFromVector(translation);
		XMStoreFloat4x4(&camera.view, view);
	}

	// TODO Move along position to target axis
	if (inputs.actionState.at(scomp::InputAction::CAM_DOLLY)) {
		if (inputs.wheelDelta > 0) {
			camera.position.z -= 1;
		} else {
			camera.position.z += 1;
		}

		XMVECTOR translation = XMLoadFloat3(&camera.position);
		XMMATRIX view = XMMatrixTranslationFromVector(translation);
		XMStoreFloat4x4(&camera.view, view);
	}

	// Reset zoom and position
	if (inputs.actionState.at(scomp::InputAction::CAM_RESET)) {
		camera.position = XMFLOAT3(0.0f, 0.0f, 6.0f);
		XMVECTOR translation = XMLoadFloat3(&camera.position);
		XMMATRIX view = XMMatrixTranslationFromVector(translation);
		XMStoreFloat4x4(&camera.view, view);
	}
}
