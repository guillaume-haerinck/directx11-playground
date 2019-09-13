#pragma once

#include "components/graphics/Shader.h"

namespace scomp {
	/**
	 * @brief Arcball camera with DOF movement support. Uses polar coordinates.
	 *
	 * @link https://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf
	 * @link https://pdfs.semanticscholar.org/5a62/af2ae372f03ff7d23708dfa111ad51221729.pdf - Six degree of freedom Control with a Two-Dimentional input device
	 * @link https://github.com/microsoft/DXUT/blob/master/Optional/DXUTcamera.h
	 * @link https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
	 */
	struct Camera {
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
		XMFLOAT3 position;
		XMFLOAT3 target;
	};
}
 