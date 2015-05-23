#pragma once

#include <CharacterUtils.h>
#include <ctype.h>

int CharacterUtils::isLetter(char _c){
	return isalpha(_c);
}

int CharacterUtils::isDigit(char _c){
	return isdigit(_c);
}

int CharacterUtils::isSymbol(char _c){
	return ((static_cast<int>(_c) >= 33 && static_cast<int>(_c) <= 47)
	|| (static_cast<int>(_c) >= 58 && static_cast<int>(_c) <= 64)
	|| (static_cast<int>(_c) >= 91 && static_cast<int>(_c) <= 96)
	|| (static_cast<int>(_c) >= 123 && static_cast<int>(_c) <= 126));
}

int CharacterUtils::isSpace(char _c){
	return static_cast<int>(_c) == 32;
}

int CharacterUtils::isSymbolLetterDigit(char _c){
	return isDigit(_c) || isLetter(_c) || isSymbol(_c);
}
