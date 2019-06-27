#pragma once
#include "MusicSymbol.h"

class Note :public MusicSymbol {
	int midi;
public:
	Note(int midi,const Duration & d):MusicSymbol(d),midi(midi) {

	}


	int getmidi() const override{
		return midi;
	}

	virtual void setmidi(int n)override {
		midi = n;
	}

	virtual void print(ostream& os) const {
		os << (*m)[getmidi()];
	}
	

	string print_xml(Miditostr & map,int mode = 0)const override {
		//mode 1 tie start
		//mode 2 tie end
		string s;
		string note = map[midi];
		char c = note[0];
		s += "<note>\n""<pitch>\n""<step>";
		s += c;
		s += "</step>\n";
		s += "<octave>" + to_string((int)midi/12-1) + "</octave>\n";
		if (note.length()==3)s += "<alter>1</alter>\n";
		s += "</pitch>\n";
		s += "<duration>" + to_string(getdur().getdur()) + "</duration>\n";
		if (mode==1) s += "<tie type=\"start\"/>\n";
		if (mode==2) s += "<tie type=\"end\"/>\n";
		s += "</note>\n";
		return s;
	}

};
