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
	 * @link https://github.com/RichieSams/thehalflingproject/blob/master/source/scene/camera.h
	 * @link https://openclassrooms.com/fr/courses/167717-creez-des-programmes-en-3d-avec-opengl/166678-controle-avance-de-la-camera-partie-1-2
	 */
	struct Camera {
		DX::XMFLOAT4X4 view;
		DX::XMFLOAT4X4 proj;
		DX::XMFLOAT3 position = { 0, 0, 6 };
		DX::XMFLOAT3 target = { 0, 0, 0 };

		float radius = 5;
		float up = 1;
		float phi = 0;
		float theta = 0;

		bool hasToBeUpdated = false;

		Camera() {
			DX::XMMATRIX projMat = DX::XMMatrixPerspectiveFovLH(DX::XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);
			DX::XMMATRIX viewMat = DX::XMMatrixTranslation(0, 0, 6);
			DX::XMStoreFloat4x4(&proj, projMat);
			DX::XMStoreFloat4x4(&view, viewMat);
		}
	};
}
