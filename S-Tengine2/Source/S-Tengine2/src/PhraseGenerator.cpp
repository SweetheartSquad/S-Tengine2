#pragma once

#include <PhraseGenerator.h>

#include <json/json.h>
#include <FileUtils.h>
#include <Log.h>
#include <regex>
#include <NumberUtils.h>

PhraseGenerator::PhraseGenerator() :
	escapeChar("*")
{
}

void PhraseGenerator::makeDatabases(std::string _databaseSrc) {
	Json::Value root;
	Json::Reader reader;

	std::string jsonLoaded = sweet::FileUtils::readFile(_databaseSrc);
	bool parsingSuccessful = reader.parse(jsonLoaded, root);

	//error check
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages());
	}else{
		Json::Value::Members termLists = root.getMemberNames();
		for (auto memberName : termLists){
			if(memberName == "escapeChar"){
				// save the escape character
				escapeChar = root[memberName].asString();
			}else{
				// parse the terms list
				std::vector<std::string> termList;
				for (Json::Value::ArrayIndex i = 0; i < root[memberName].size(); ++i) {
					std::string p = root[memberName][i].asString();
					termList.push_back(p);
				}
				terms[memberName] = termList;
			}
		}
	}
}


std::string PhraseGenerator::replaceWords(std::string phrase) {
	while(phrase.find(escapeChar) != std::string::npos){
		std::regex r("\\" + escapeChar + "(.+?)\\" + escapeChar);
		std::smatch match;
		const std::string constPhrase = phrase;

		std::string word;
		std::string replacer;
		std::string replacee;
		int choice;

		if (std::regex_search(constPhrase, match, r)){
			word = match[1];
		}

		choice = sweet::NumberUtils::randomInt(0, terms[word].size() - 1);
		replacee = "\\" + escapeChar + word + "\\" + escapeChar;
		replacer = terms[word][choice];
		std::regex rg(replacee);

		phrase = std::regex_replace(constPhrase, rg, replacer);
	}

	return phrase;
}

/*std::string PD_InsultGenerator::fixVowels() {
	// TODO: implement this
	// possible places to start:
	//http://stackoverflow.com/questions/12798162/a-vs-an-any-php-library-can-handle-this-grammar-rule
	//http://dustwell.com/a-or-an-in-front-of-a-word.html
}*/
