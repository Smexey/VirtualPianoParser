#pragma once
#include <iostream>
#include <map>
#include "Duration.h"

using namespace std;
typedef map<int, string> Miditostr;
class MusicSymbol {

protected:
	Miditostr* m=nullptr;
	Duration dur;
public:
	MusicSymbol(const Duration & d):dur(d) {
	}

	virtual ~MusicSymbol() {
	}

	Duration getdur() const {
		return dur;
	}
	virtual int getmidi()const = 0;

	virtual void setmidi(int n) = 0;

	virtual void print(ostream& os) const = 0;

	void setmap(Miditostr & m) {
		this->m = &m;
	}


	virtual string print_xml(map<int, string> & map, int mode ) const = 0;


	friend ostream& operator<<(ostream& os, const MusicSymbol& m){
		if (m.m == nullptr) throw Greska("uninitialized musicsymbol print map");
		m.print(os);
		
		return os;
	}
};
