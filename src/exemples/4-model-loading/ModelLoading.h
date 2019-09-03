#pragma once

#include "exemples/IExemple.h"
#include "components/graphics/Shader.h"
#include "core/Context.h"
#include "core/StepTimer.h"

namespace exemple {
	class ModelLoading : public IExemple {
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
