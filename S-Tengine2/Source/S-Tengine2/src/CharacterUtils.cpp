#pragma once

#include <CharacterUtils.h>
#include <ctype.h>

int sweet::CharacterUtils::isLetter(unsigned char _c){
	return isalpha(_c);
}

int sweet::CharacterUtils::isDigit(unsigned char _c){
	return isdigit(_c);
}

int sweet::CharacterUtils::isSymbol(unsigned char _c){
	return ((static_cast<int>(_c) >= 33 && static_cast<int>(_c) <= 47)
	|| (static_cast<int>(_c) >= 58 && static_cast<int>(_c) <= 64)
	|| (static_cast<int>(_c) >= 91 && static_cast<int>(_c) <= 96)
	|| (static_cast<int>(_c) >= 123 && static_cast<int>(_c) <= 126));
}

int sweet::CharacterUtils::isSpace(unsigned char _c){
	return static_cast<int>(_c) == 32;
}

int sweet::CharacterUtils::isSymbolLetterDigit(unsigned char _c){
	return isDigit(_c) || isLetter(_c) || isSymbol(_c);
}
