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
		camera.theta += inputs.delta.x * 0.03;
		camera.phi += inputs.delta.y * 0.03;

		// Keep phi within -2PI to +2PI for easy 'up' comparison
		if (camera.phi > XM_2PI) {
			camera.phi -= XM_2PI;
		} else if (camera.phi < -XM_2PI) {
			camera.phi += XM_2PI;
		}

		// If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
		if ((camera.phi > 0 && camera.phi < XM_PI) || (camera.phi < -XM_PI && camera.phi > -XM_2PI)) {
			camera.up = 1.0f;
		} else {
			camera.up = -1.0f;
		}

		camera.hasToBeUpdated = true;
	}

	// Move along vertical plane
	if (inputs.actionState.at(scomp::InputAction::CAM_PAN)) {
		XMVECTOR position = XMVectorAdd(XMLoadFloat3(&camera.target), XMLoadFloat3(&camera.position));
		XMVECTOR look = XMVector3Normalize(DirectX::XMVectorSubtract(XMLoadFloat3(&camera.target), position));
		XMVECTOR worldUp = XMVectorSet(0.0f, camera.up, 0.0f, 0.0f);

		XMVECTOR right = XMVector3Cross(look, worldUp);
		XMVECTOR up = XMVector3Cross(look, right);

		XMVECTOR target = XMVectorAdd(XMLoadFloat3(&camera.target), DirectX::XMVectorAdd(XMVectorScale(right, inputs.delta.x * 0.03), DirectX::XMVectorScale(up, inputs.delta.y * 0.03)));
		XMStoreFloat3(&camera.target, target);

		camera.hasToBeUpdated = true;
	}

	// Change arcball radius
	if (inputs.actionState.at(scomp::InputAction::CAM_DOLLY)) {
		if (inputs.wheelDelta > 0 && camera.radius >= 2) {
			camera.radius -= 1;
		} else if (inputs.wheelDelta < 0) {
			camera.radius += 1;
		}

		camera.hasToBeUpdated = true;
	}

	// Reset zoom and position
	if (inputs.actionState.at(scomp::InputAction::CAM_RESET)) {
		camera.phi = 0;
		camera.theta = 0;
		camera.radius = 10;
		camera.up = 1;
		camera.target = XMFLOAT3(0, 0, 0);

		camera.hasToBeUpdated = true;
	}

	// Update camera position
	if (camera.hasToBeUpdated == true) {
		camera.position.x = camera.radius * sinf(camera.phi) * sinf(camera.theta);
		camera.position.y = camera.radius * cosf(camera.phi);
		camera.position.z = camera.radius * sinf(camera.phi) * cosf(camera.theta);

		XMVECTOR eye = XMLoadFloat3(&camera.position);
		XMVECTOR target = XMLoadFloat3(&camera.target);
		XMVECTOR up = XMVectorSet(0, camera.up, 0, 1);

		XMMATRIX view = XMMatrixLookAtLH(eye, target, up);
		XMStoreFloat4x4(&camera.view, view);

		camera.hasToBeUpdated == false;
	}
}
