#include "ConvertString.h"
#include <cassert>
#include <dxgi1_6.h>

std::wstring MNE::Util::ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

std::string MNE::Util::ConvertToString(const wchar_t* name)
{
	std::wstring ws = name;

	size_t inLen = (size_t)ws.length();
	size_t outLen = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)inLen, 0, 0, 0, 0);

	std::string result(outLen, '\0');
	if (outLen) WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)inLen, &result[0], (int)outLen, 0, 0);

	return result;
}

std::string MNE::Util::GetDirectoryPath(const std::string& origin)
{
	int ind = (int)origin.find_last_of('/') + 1;
	return origin.substr(0, ind);
}

std::string MNE::Util::GetFileName(const std::string& origin)
{
	if (origin.find("/") == std::string::npos) return origin;
	int offset = (int)origin.find_last_of('/') + 1;
	int ind = (int)origin.length() - offset;
	return origin.substr(offset, ind);
}

std::string MNE::Util::GetString(const std::string& origin, const std::string& findFirstName)
{
	if (origin.find(findFirstName) == std::string::npos) return origin;

	int32_t offset = static_cast<uint32_t>(origin.find_first_of(findFirstName)) + 1;
	//int32_t ind = static_cast<uint32_t>(origin.length()) - offset;

	return origin.substr(offset);
}
