#include "pch.h"
#include "PBRDirectionalLight.h"

#include "graphics/ConstantBuffer.h"
#include "factories/entities/ModelFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "components/physics/Transform.h"
#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Lights.h"

namespace pbrExample {
	PBRDirectionalLight::PBRDirectionalLight(Context& context) : m_ctx(context) {
		
	}

	PBRDirectionalLight::~PBRDirectionalLight()
	{
	}

	void PBRDirectionalLight::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void PBRDirectionalLight::ImGuiUpdate() {
		
	}
}
