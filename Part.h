#pragma once
#include "Measure.h"
#include "MidiFile.h"
#include "Chord.h"

class Part {
	friend class Kompozicija;

	std::vector<Measure*> arr;

public:
	typedef std::vector<Measure*>::iterator Iter;
	Part(){}

	~Part() {
		for (auto i : arr) {
			delete i;
		}
	}

	void setmap(Miditostr & m) {
		for (auto i : arr) {
			i->setmap(m);
		}
	}


	void setnote(int measureind, int noteind, int midi) {

	}

	void insert(Measure* m) {
		arr.push_back(m);
	}

	Iter begin() {
		return arr.begin();
	}

	Iter end() {
		return arr.end();
	}

	Measure *& operator[](int n) {
		return arr[n];
	}

	string print_xml(Miditostr & map, string str, const Duration & beat);

	friend std::ostream& operator << (std::ostream& os, const Part& p) {
		os << " |";
		for (auto i : p.arr) {
			os << *i;
			os << "|";
		}
		return os;
	}

	void print_midi(int tpq, smf::MidiFile & outputfile);

};