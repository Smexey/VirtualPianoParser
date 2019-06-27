#pragma once
#include "MusicSymbol.h"

class Pause : public MusicSymbol {
	int midi;
public:
	Pause(const Duration & d) :MusicSymbol(d), midi(0) {
		//midi = 0 za sve pauze, razlicita samo duzina
	}


	int getmidi() const override {
		return midi;
	}

	void setmidi(int n) override {
		throw Greska("cant set pause midi");
	}

	virtual void print(ostream& os)const {
		if (dur.getdur() == 1) {
			os << (char)170;
		}
		else {
			os << (char)206;
		}
	}

	string print_xml(Miditostr & map, int mode = 0)const override {
		return string("<note>\n<rest/>\n<duration>" + to_string(getdur().getdur()) + "</duration>\n</note>\n");
	}
};