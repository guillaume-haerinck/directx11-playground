#pragma once

#include "core/Context.h"
#include "graphics/DXObjects.h"
#include "graphics/Renderer.h"
#include "graphics/RenderCommand.h"
#include "exemples/IExemple.h"

/**
 * @brief Base singleton class to create the application and run it
 */
class App {
public:
	App(HINSTANCE& hInstance);
	~App();

	/**
	 * @brief Run the application
	 * @note Must be called once by frame
	 */
	void Update(float dt);

	ID3D11Debug* GetDebugDevice() { return m_debugDevice; }

private:
	void initWindow(HINSTANCE& hInstance);
	void initDirectX11();
	void initImGui();

private:
	HWND m_hwnd;
	const char* m_className;

	DXObjects m_dxo;
	Context m_ctx;
	ID3D11Debug* m_debugDevice;
	std::unique_ptr<IExemple> m_activeExemple;
};
