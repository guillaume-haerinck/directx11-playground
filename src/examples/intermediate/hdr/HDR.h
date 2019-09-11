#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace interExample {
	class HDR : public IExample {
	public:
		HDR(Context& context);
		virtual ~HDR();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
