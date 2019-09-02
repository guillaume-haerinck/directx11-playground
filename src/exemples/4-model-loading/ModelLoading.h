#pragma once

#include "exemples/IExemple.h"
#include "core/Context.h"

namespace exemple {
	class ModelLoading : public IExemple {
	public:
		ModelLoading(Context& context);
		virtual ~ModelLoading();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		Context& m_ctx;
	};
}
