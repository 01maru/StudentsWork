#pragma once
#include <string>

namespace Util {
	
	std::wstring ToWideString(const std::string& str);

	std::string ConvertToString(const wchar_t* name);

	std::string GetDirectoryPath(const std::string& origin);
	std::string GetFileName(const std::string& origin);
}

