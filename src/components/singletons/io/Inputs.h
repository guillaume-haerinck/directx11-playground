#pragma once

namespace scomp {
	enum InputAction {
		TILT = 0,
		_ACTION_MAX
	};

	struct Inputs {
		std::array<bool, InputAction::_ACTION_MAX> actionState;
		XMFLOAT2 mousePos;
	};
}
