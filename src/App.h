#pragma once

#include "graphics/Renderer.h"
#include "graphics/RenderCommand.h"

class App {
public:
	App(HINSTANCE& hInstance);
	~App();

	/**
	 * @brief Run the application
	 * @note Must be called once by frame
	 */
	void Update(float dt);

	HWND getHwnd() const { return m_hwnd; }

private:
	HWND m_hwnd;
	WNDCLASSEX m_wc;
	const char* m_className;
};
