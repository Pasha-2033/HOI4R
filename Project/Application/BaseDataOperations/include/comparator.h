#pragma once
#include <Esent.h>
//добавить inline
namespace comparator {
	bool equalwchars(WCHAR* word1, WCHAR* word2);
	bool equalwchars(const WCHAR* word1, const WCHAR* word2);
}