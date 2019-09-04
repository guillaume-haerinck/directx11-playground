#pragma once

#include "examples/IExample.h"
#include "components/graphics/Shader.h"
#include "core/Context.h"
#include "core/StepTimer.h"

namespace exemple {
	class ModelLoading : public IExample {
	public:
		ModelLoading(Context& context);
		virtual ~ModelLoading();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		DX::StepTimer m_timer;
		comp::ConstantBuffer m_VSCB0;
	};
}
