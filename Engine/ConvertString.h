#pragma once
#include <string>

/**
* @file ConvertString.h
* @brief stringの変換関数をまとめたファイル
*/

namespace Util {

	/**
	* @fn ToWideString(const std::string&)
	* string -> wstringに変換する関数
	* @param str wstringに変換する文字列
	* @return wstringに変換後の文字列
	*/
	std::wstring ToWideString(const std::string& str);

	/**
	* @fn ConvertToString(const wchar_t*)
	* wchar_t* -> stringに変換する関数
	* @param name stringに変換する文字列
	* @return stringに変換後の文字列
	*/
	std::string ConvertToString(const wchar_t* name);

	/**
	* @fn GetDirectoryPath(const std::string&)
	* originからディレクトリーパスだけを返す関数
	* @param origin ディレクトリーパスを調べたい文字列
	* @return ディレクトリーパス
	*/
	std::string GetDirectoryPath(const std::string& origin);
	/**
	* @fn GetFileName(const std::string&)
	* originからファイル名だけを返す関数
	* @param origin ファイル名を調べたい文字列
	* @return ファイル名
	*/
	std::string GetFileName(const std::string& origin);
}

