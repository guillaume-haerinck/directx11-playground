#pragma once

/**
 * @brief Helper class for DirectX exceptions
 */
class DXException : public std::exception {
public:
	DXException(int line, const char* file, HRESULT hr) noexcept;
	virtual const char* what() const noexcept override;

	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

	static std::string TranslateErrorCode(HRESULT hr) noexcept;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;

private:
	int line;
	std::string file;
	HRESULT hr;

protected:
	mutable std::string whatBuffer;
};

namespace DXC {
	inline void ThrowIfFailed(int line, const char* file, HRESULT hr) {
		if (FAILED(hr)) {
			// Set a breakpoint on this line to catch DirectX API errors
			throw DXException(line, file, hr);
		}
	}
}

#define DX_LAST_ERROR_EXCEPTION DXException(__LINE__, __FILE__, GetLastError())
#define CALL_INFO __LINE__,__FILE__