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

	// ArcBall rotation
	if (inputs.actionState.at(scomp::InputAction::CAM_ORBIT)) {

		camera.position.x -= inputs.delta.x * 0.01;
		camera.position.y -= inputs.delta.y * 0.01;

		XMVECTOR eye = XMLoadFloat3(&camera.position);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up = XMVectorSet(0, 1, 0, 1);

		XMMATRIX view = XMMatrixLookAtLH(eye, target, up);
		XMStoreFloat4x4(&camera.view, view);
	}

	if (inputs.actionState.at(scomp::InputAction::CAM_PAN)) {

	}

	if (inputs.actionState.at(scomp::InputAction::CAM_DOLLY)) {

	}

	if (inputs.actionState.at(scomp::InputAction::CAM_RESET)) {

	}
}
