#include "../include/comparator.h"
//заметки в заголовке
bool comparator::equalwchars(WCHAR* word1, WCHAR* word2) {
	size_t size = wcslen(word1);
	if (size != wcslen(word2)) return false;
	for (size_t i = 0; i < size; i++) {
		if (word1[i] != word2[i]) return false;
	}
	return true;
}
bool comparator::equalwchars(const WCHAR* word1, const WCHAR* word2) {
	return equalwchars((WCHAR*)word1, (WCHAR*)word2);
}