#pragma once
#include "MusicSymbol.h"

class Chord :public MusicSymbol {

	int *arr;
	int size;
public:
	Chord(int* arr,int size,const Duration & d):MusicSymbol(d),arr(arr),size(size) {
	}
	~Chord() {
		delete[] arr;
	}

	int getmidi()const override{
		return -1*size;//chord!!!
	}

	virtual void setmidi(int n) override {
		//ne sme set
		throw Greska("cant set chord midi");
	}

	virtual void print(ostream& os)const {
		os << (*m)[arr[0]];
	}


	int operator[](int i) {
		return arr[i];
	}

	string print_xml(Miditostr & map, int mode = 0)const override {
		string s;
		for (size_t i = 0; i < size; i++){
			string note = map[arr[i]];
			char c = note[0];
			s += "<note>\n""<pitch>\n""<step>";
			s += c;
			s += "</step>\n";
			s += "<octave>" + to_string((int)arr[i] / 12 -1 ) + "</octave>\n";
			if (note.length() == 3)s += "<alter>1</alter>\n";
			s += "</pitch>\n";
			s += "<duration>" + to_string(getdur().getdur()) + "</duration>\n";
			if (mode == 1)s += "<tie type=\"start\"/>\n";
			if (mode == 2)s += "<tie type=\"end\"/>\n";
			s += "</note>\n";
			if (i != 0) s += "<chord/>\n";
		}
		return s;
	}

};