#pragma once

// TODO check if the pointer ref count is incremented when copying the struct

/*
 * @brief DirectX Objects needed for any interaction with the API
 */
struct DXObjects {
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencil;
};
