#pragma once

#include "DXObjects.h"

/**
 * @brief Issue draw commands to Direct3D API
 */
class RenderCommand {
public:
	RenderCommand(DXObjects dxObjects);

	/**
	 * @brief Clear the back buffer
	 */
	void Clear() const;

	/**
	 * @brief Swap front and back buffers
	 */
	void Swap() const;

private:
	DXObjects m_dxo;
};
