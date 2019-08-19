#pragma once

#include "RenderCommand.h"
#include "DXObjects.h"

/**
 * @brief Draw a 3d scene on the screen
 * @note Never calls DirectX directly. It uses renderCommand to interact with the API
 */
class Renderer {
public:
	Renderer(DXObjects dxObjects);
	~Renderer();

	/**
	 * @brief Setup the camera and the scene content
	 * @note Must be called once for each frame
	 *
	 * TODO take a camera on entry
	 */
	void BeginScene();

	/**
	 * @brief Issue a draw call to the GPU through renderCommand 
	 *		  (In the future, it might add it to a render queue)
	 * @note Must be called between BeginScene() and EndScene()
	 *
	 * TODO take a material, a buffer and a transform
	 */
	void Submit();

	/**
	 * @brief Cleanup the scene and sends what remains to the GPU
	 * @note Must be called once for each frame
	 */
	void EndScene();

private:
	RenderCommand m_rc;
};
