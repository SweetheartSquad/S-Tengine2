#pragma once
namespace sweet{
class CharacterUtils{
public:
	static int isLetter(unsigned char _c);
	static int isDigit(unsigned char _c);
	static int isSymbol(unsigned char _c);
	static int isSpace(unsigned char _c);
	static int isSymbolLetterDigit(unsigned char _c);
};
};