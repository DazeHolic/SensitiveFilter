
#pragma once

#include "aho_corasick.h"



namespace SF
{
	class SensitiveFilter
	{
	public:
		SensitiveFilter();
		~SensitiveFilter();

		bool Init(std::string strPath);

		bool CheckValid(std::string strWords);

	private:
		void LoadFile(std::string strPath);
		bool CheckByCondition(std::string strWords, bool bLetter, bool bNumber, bool bChinese);

		bool unicode_is_letter(int index);
		bool unicode_is_number(int index);
		bool unicode_is_chinese(int index);
		std::wstring Utf8ToUnicode(const std::string& str);


	private:
		aho_corasick::basic_trie<wchar_t> m_trie;
	};

}
