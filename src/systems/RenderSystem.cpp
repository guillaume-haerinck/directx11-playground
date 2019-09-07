#include "pch.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	// TODO Get camera from singleton component and update its constant buffer at each frame
	// TODO Get lights from singleton component and update its constant buffer when there is a change
	// TODO Get materials from component and update its constant buffer when there is a change
}
