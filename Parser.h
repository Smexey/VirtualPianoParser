#pragma once
#include <fstream>
#include <map>
#include <string>
#include <regex>
#include "Greska.h"
#include "Composition.h"
#include "Pause.h"
#include "Chord.h"
#include "Note.h"
using namespace std;
typedef map<int, string> Miditostr;
typedef map<char, pair<string, int>> Notesmap;

class Parser {

public:
	Parser() {}

	Composition& parsecomposition(const char * path, const Duration & d);
	
	Notesmap& parsemap(const char * path);

	Notesmap& getnotesmap() {
		return *nmap;
	}

	Miditostr& getmiditostr() {
		return *miditos;
	}

private:
	Notesmap * nmap;
	Miditostr * miditos;

	void pushtocomp(Composition & comp, Measure * & measmelody, Measure*& measbass, const Duration & d, int mode = 1);

	void insertregularnote(Measure*& measmelody, Measure *& measbass, string& str, Composition & comp, const Duration& d, int mode = 1);
};