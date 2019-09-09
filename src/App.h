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
	// Once in a lifetime init
	void initWindow(HINSTANCE& hInstance);
	void initDirectX11();
	void initImGui();

	// Once per exemple change init
	void initGraphicSingletonEntity();
	void initIOSingletonEntity();

	template<typename T>
	void resetAppTo() {
		m_ctx.registry.reset();
		initGraphicSingletonEntity();
		initIOSingletonEntity();
		m_activeExemple.reset();
		m_activeExemple = std::make_unique<T>(m_ctx);
	}

	// Once per frame
	void renderMenu();

private:
	HWND m_hwnd;
	const char* m_className;

	DXObjects m_dxo;
	Context m_ctx;
	std::shared_ptr<ID3D11Debug> m_debugDevice;
	std::unique_ptr<IExample> m_activeExemple;
};
