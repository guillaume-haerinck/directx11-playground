#include "pch.h"
#include "CameraSystem.h"

#include "components/singletons/io/Inputs.h"

CameraSystem::CameraSystem(Context& context) : m_ctx(context)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Update() {
	/*
		TODO orbit around center (look at) when press middle mouse or right click
		TODO move camera with left click (also change its center of orbit)
		TODO zoom in and out with mousewheel
		TODO recenter view with double click
		TODO select object on left click
	*/

	auto ioEntity = m_ctx.singletonComponents.at(SingletonComponents::IO);
	scomp::Inputs inputs = m_ctx.registry.get<scomp::Inputs>(ioEntity);
}
