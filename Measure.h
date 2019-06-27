#pragma once
#include "MusicSymbol.h"
#include <list>
#include <vector>


class Measure {
	friend class Kompozicija;
	bool linked = false;
	int dur = 0;
	int maxdur;

	std::vector<MusicSymbol*> arr;


public:
	typedef std::vector<MusicSymbol*>::iterator Iter;

	Measure(const Duration & d) {
		maxdur = d.getdur();
	}
	~Measure() {
		for (auto i : arr) {
			delete i;
		}
	}

	void setmap(Miditostr & m) {
		for (auto i : arr) {
			i->setmap(m);
		}
	}

	int getdur()const {
		return dur;
	}

	int getmaxdur()const {
		return maxdur;
	}

	Measure & setlinked(bool l) {
		linked = l;
		return *this;
	}

	bool getlinked()const {
		return linked;
	}

	MusicSymbol *& operator[](int n) {
		return arr[n];
	}


	Measure& insert(MusicSymbol* s) {
		arr.push_back(s);
		dur += s->getdur().getdur();
		return *this;
	}

	Measure& insert(MusicSymbol* s,bool link) {
		arr.push_back(s);
		if(!link)
			dur += s->getdur().getdur();
		return *this;
	}


	Iter begin() {
		return arr.begin();
	}

	Iter end() {
		return arr.end();
	}

	friend std::ostream& operator << (std::ostream& os, const Measure& m) {

		for (auto i : m.arr) {
			os.setf(ios::right);
			os.fill(' ');	
			os.width(4);	
			os << *i;
		}
		return os;
	}
};