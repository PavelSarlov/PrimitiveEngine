#include "ErrorLogger.h"
#include <comdef.h>
#include <iostream>

void ErrorLogger::log(std::string message)
{
	//std::string error_message = "Error: " + message;
	//MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
	std::cout << "ERROR: " << message << std::endl;
}

void ErrorLogger::log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + std::wstring(message.begin(), message.end()) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
