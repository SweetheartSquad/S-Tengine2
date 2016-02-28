#pragma once

#include <node/Node.h>
#include <NumberUtils.h>

#include <string>
#include <map>

// A generic phrase generator which uses regex to replace pre-defined
// escaped terms with random entries from an externally-loaded JSON database
//
// Example term database:
//
//	{
//		"escapeChar": "*",
//		"sentence": [
//			"That's a *adjective* *noun*."
//			"You should *verb*!"
//		],
//		"adjective": [
//			"bad",
//			"good"
//		],
//		"noun": [
//			"dog",
//			"cat"
//		],
//		"verb": [
//			"run",
//			"walk"
//		]
//	}
class PhraseGenerator : public Node{
public:
	PhraseGenerator();
protected:
	// individual terms are stored in vectors which can be accessed by their name
	std::map<std::string, sweet::ShuffleVector<std::string>> terms;

	// character used to indicate which terms need to be replaced
	// e.g. if the escapeChar is "*", a phrase might be stored as
	// "Have a *adjective* day", and "*adjective*" would get replaced
	// with a term from the "adjective" list in the database
	//
	// NOTE: The escapeChar is loaded from the "escapeChar" property of
	// the term database, but will be set to "*" if not found
	// Also note that it doesn't have to be an individual character
	std::string escapeChar;

	// replaces any escaped sequences within _phrase with random terms of the appropriate type and returns the result
	// this is iterative, and will occur as many times as necessary
	// e.g. *phrase* -> The *noun* was *adjective*. -> The dog was good.
	std::string replaceWords(std::string _phrase);

	// accounts for the "a" vs. "an" problem
	//std::string fixVowels(std::string phrase);
	
	// loads a json file stored at _databaseSrc into the map of terms
	void makeDatabases(std::string _databaseSrc);
};
