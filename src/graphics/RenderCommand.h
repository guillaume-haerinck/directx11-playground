#pragma once

#include "DXObjects.h"

/**
 * @brief Issue draw commands to Direct3D API
 * @note The DirectX device is created here
 */
class RenderCommand {
public:
	RenderCommand(DXObjects dxObjects);

	void Clear() const;
	void Swap() const;

	// TEMP
	void DrawTriangle();

private:
	DXObjects m_dxo;
};
