#pragma once

#include "core/Context.h"
#include "graphics/DXObjects.h"
#include "graphics/Renderer.h"
#include "graphics/RenderCommand.h"
#include "examples/IExample.h"

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

	std::shared_ptr<ID3D11Debug> GetDebugDevice() { return m_debugDevice; }

private:
	void initWindow(HINSTANCE& hInstance);
	void initDirectX11();
	void initImGui();
	void initGraphicSingletonEntity();

private:
	HWND m_hwnd;
	const char* m_className;

	DXObjects m_dxo;
	Context m_ctx;
	std::shared_ptr<ID3D11Debug> m_debugDevice;
	std::unique_ptr<IExample> m_activeExemple;
};
