#include "ConvertString.h"
#include <cassert>
#include <dxgi1_6.h>

std::wstring Util::ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

std::string Util::ConvertToString(const wchar_t* name)
{
	std::wstring ws = name;

	size_t inLen = (size_t)ws.length();
	size_t outLen = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)inLen, 0, 0, 0, 0);

	std::string result(outLen, '\0');
	if (outLen) WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)inLen, &result[0], (int)outLen, 0, 0);

	return result;
}

std::string Util::GetDirectoryPath(const std::string& origin)
{
	int ind = (int)origin.find_last_of('/');
	ind++;
	return origin.substr(0, ind);
}
