#pragma once

#include "graphics/DXObjects.h"
#include "graphics/Renderer.h"
#include "graphics/RenderCommand.h"
#include "exemples/i-exemple.h"

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

private:
	void initWindow(HINSTANCE& hInstance);
	void initDirectX11();
	void initImGui();

private:
	HWND m_hwnd;
	const char* m_className;

	DXObjects m_dxo;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<RenderCommand> m_rcommand;
	std::unique_ptr<IExemple> m_activeExemple;
};
