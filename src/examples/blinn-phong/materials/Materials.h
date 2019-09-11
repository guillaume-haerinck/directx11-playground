#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace phongExample {
	class Materials : public IExample {
	public:
		Materials(Context& context);
		virtual ~Materials();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
