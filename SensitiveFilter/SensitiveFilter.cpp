
#include <io.h>
#include <fstream>
#include "SensitiveFilter.h"
#include <windows.h>

namespace SF
{

	SensitiveFilter::SensitiveFilter()
	{
	}

	SensitiveFilter::~SensitiveFilter()
	{
	}


	bool SensitiveFilter::Init(std::string strPath)
	{
		std::string searchPath = strPath + "\\*.txt";
		struct _finddata_t fileinfo;
		auto handle = _findfirst(searchPath.c_str(), &fileinfo);
		if (handle == -1)
		{
			return false;
		}

		do
		{
			std::string filePath = strPath + "\\" + fileinfo.name;
			LoadFile(filePath);
		} while (!_findnext(handle, &fileinfo));

		m_trie.load_finish();
		_findclose(handle);
		return true;
	}

	bool SensitiveFilter::CheckValid(std::string strWords)
	{
		return CheckByCondition(strWords, true, true, true) && CheckByCondition(strWords, false, false, true);
	}

	void SensitiveFilter::LoadFile(std::string strPath)
	{
		std::ifstream infile;
		infile.open(strPath);

		std::string strContent;
		while (getline(infile, strContent))
		{
			std::wstring unicode_codepoints = Utf8ToUnicode(strContent);
			m_trie.insert(unicode_codepoints);
		}
	}

	// 先过滤后检测(bLetter=true表示保留字母，bNumber=true表示保留阿拉伯数字，bChinese=true表示保留中文)
	bool SensitiveFilter::CheckByCondition(std::string strWords, bool bLetter, bool bNumber, bool bChinese)
	{
		std::wstring strUnicodeWords = Utf8ToUnicode(strWords);
		std::wstring strFilterAll;
		for (int j = 0; j < strUnicodeWords.size(); j++)
		{
			int index = strUnicodeWords[j];
			if (bLetter && unicode_is_letter(index)
				|| bNumber && unicode_is_number(index)
				|| bChinese && unicode_is_chinese(index))
			{
				strFilterAll.push_back(index);
			}
		}
		return !m_trie.check_exist(strFilterAll);
	}

	bool SensitiveFilter::unicode_is_letter(int index)
	{
		return index >= 0x41 && index <= 0x5a || index >= 0x61 && index <= 0x7a;
	}

	bool SensitiveFilter::unicode_is_number(int index)
	{
		return index >= 0x30 && index <= 0x39;
	}

	bool SensitiveFilter::unicode_is_chinese(int index)
	{
		return index >= 0x4e00 && index <= 0x9fa5;
	}

	std::wstring SensitiveFilter::Utf8ToUnicode(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

}