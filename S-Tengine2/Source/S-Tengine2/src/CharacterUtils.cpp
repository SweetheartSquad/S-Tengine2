#include <CharacterUtils.h>

bool CharacterUtils::isLetter(char _c){
	return ((static_cast<int>(_c) >= 65 && static_cast<int>(_c) <= 90)
		||(static_cast<int>(_c) >= 97 && static_cast<int>(_c) <= 122));
}

bool CharacterUtils::isDigit(char _c){
	return (static_cast<int>(_c) >= 48 && static_cast<int>(_c) <= 57);
}

bool CharacterUtils::isSymbol(char _c){
	return ((static_cast<int>(_c) >= 33 && static_cast<int>(_c) <= 47)
	|| (static_cast<int>(_c) >= 58 && static_cast<int>(_c) <= 64)
	|| (static_cast<int>(_c) >= 91 && static_cast<int>(_c) <= 96)
	|| (static_cast<int>(_c) >= 123 && static_cast<int>(_c) <= 126));
}

bool CharacterUtils::isSymbolLetterDigit(char _c){
	return isDigit(_c) || isLetter(_c) || isSymbol(_c);
}