#include "pch.h"
#include "DXException.h"

DXException::DXException(int line, const char* file, HRESULT hr) noexcept
: line(line), file(file), hr(hr) {}

const char* DXException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* DXException::GetType() const noexcept {
	return "DirectX Exception";
}

int DXException::GetLine() const noexcept {
	return line;
}

const std::string& DXException::GetFile() const noexcept {
	return file;
}

std::string DXException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

HRESULT DXException::GetErrorCode() const noexcept {
	return hr;
}

std::string DXException::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
}

std::string DXException::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	if (nMsgLen == 0) {
		return "Unidentified error code";
	}

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}
