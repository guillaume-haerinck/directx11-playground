#pragma once

#include "ISystem.h"
#include "core/Context.h"

class CameraSystem : public ISystem {
public:
	CameraSystem(Context& context);
	virtual ~CameraSystem();

	void Update() override;

private:
	Context& m_ctx;
};
