#pragma once

class IExemple {
public:
	virtual ~IExemple() {};

	/*
	 * @brief Update the geometry and sends it to the render queue
	 */
	virtual void Update() = 0;

	/*
	 * @brief Update ImGui Interface
	 */
	virtual void ImGuiUpdate() = 0;
};
