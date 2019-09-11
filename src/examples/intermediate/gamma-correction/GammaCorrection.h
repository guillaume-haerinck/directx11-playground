#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace interExample {
	class GammaCorrection : public IExample {
	public:
		GammaCorrection(Context& context);
		virtual ~GammaCorrection();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}