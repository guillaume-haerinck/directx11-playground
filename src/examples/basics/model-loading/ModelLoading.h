#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace basicExample {
	class ModelLoading : public IExample {
	public:
		ModelLoading(Context& context);
		virtual ~ModelLoading();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::unique_ptr<ISystem>> m_systems;
	};
}
