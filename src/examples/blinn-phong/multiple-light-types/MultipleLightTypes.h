#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace phongExample {
	class MultipleLightTypes : public IExample {
	public:
		MultipleLightTypes(Context& context);
		virtual ~MultipleLightTypes();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
