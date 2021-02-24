#include <iostream>
#include "SensitiveFilter.h"

int main()
{
    SF::SensitiveFilter filter;
    filter.Init("./SensitiveWord");

	// 淘宝 为敏感词
	std::string strCheck = u8"我@淘#！e2*&宝现场,问阿34#@桑的歌";

	auto ret = filter.CheckValid(strCheck);
	if (!ret)
	{
		std::cout << "发现敏感字!\n";
	}
	else
	{
		std::cout << "未发现敏感字\n";
	}

	std::string s;
	getline(std::cin, s);
}
