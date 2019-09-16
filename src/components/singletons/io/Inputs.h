#pragma once

namespace scomp {
	enum InputAction {
		CAM_ORBIT = 0,
		CAM_PAN,
		CAM_DOLLY,
		CAM_RESET,
		_ACTION_MAX
	};

	struct Inputs {
		std::array<bool, InputAction::_ACTION_MAX> actionState;
		DX::XMFLOAT2 mousePos;
		DX::XMFLOAT2 delta;
		short wheelDelta;
	};
}
