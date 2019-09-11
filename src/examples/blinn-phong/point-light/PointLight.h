#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace phongExample {
	class PointLight : public IExample {
	public:
		PointLight(Context& context);
		virtual ~PointLight();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
		entt::entity m_litEntity;
		entt::entity m_lightEntity;
	};
}
