#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace pbrExample {
	class PBRDirectionalLight : public IExample {
	public:
		PBRDirectionalLight(Context& context);
		virtual ~PBRDirectionalLight();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
