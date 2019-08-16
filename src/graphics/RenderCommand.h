#pragma once

/**
 * @brief Issue draw commands to Direct3D API
 * @note The DirectX device is created here
 */
class RenderCommand {
public:
	RenderCommand(HWND& hWnd);

	void Clear() const;
	void Swap() const;

	// TEMP
	void DrawTriangle();

public:
	ID3D11Device* GetDevice() const { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() const { return m_context.Get(); }

private:
	DXGI_SWAP_CHAIN_DESC m_sd;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
};
