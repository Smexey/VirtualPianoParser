#pragma once
#include "Part.h"
#include <map>


using namespace std;
typedef map<int, string> Miditostr;
typedef map<char, pair<string, int>> Notesmap;
typedef unsigned short Colour;

class Composition {
	Part bass;
	Part melody;
	Duration beat;
public:
	
	Composition(const Duration & beat):beat(beat) {
	}

	Composition(const Composition &c,const Duration & beat):bass(c.bass),melody(c.melody),beat(beat) {

	}
	void setmap(Miditostr & m) {
		bass.setmap(m);
		melody.setmap(m);
	}

	void setnote(int measureind, int noteind, int midi);
	
	friend std::ostream& operator << (std::ostream& os, const Composition& p) {
		
		os << p.beat<<p.melody << endl;
		os << p.beat<<p.bass << endl;
		return os;
	}

	const Duration & getbeat()const {
		return beat;
	}
	Part::Iter bassbegin() {
		return bass.begin();
	}

	Part::Iter melobegin() {
		return melody.begin();
	}


	Composition & insertbass(Measure * m) {
		bass.insert(m);
		return *this;
	}

	Composition & insertmelody(Measure * m) {
		melody.insert(m);
		return *this;
	}

	Composition & changeoct(int delta);

	Composition& print_midi(Miditostr & map, string outfilename, int speed = 1);

	Composition& print_xml(Miditostr & nmap,string outfilename);

	Composition& print_bmp(int w,string outfilename);

private:
	Colour getr(int midi) {
		if (midi == 0) return 255;
		return mapoct(getr_oct4(midi), midi);
	}

	Colour getg(int midi) {
		if (midi == 0) return 255;
		return mapoct(getg_oct4(midi), midi);
	}

	Colour getb(int midi) {
		if (midi == 0) return 255;
		return mapoct(getb_oct4(midi), midi);
	}

	Colour getr_oct4(int midi)const {
		int n = midi % 12;
		if (n == 3 || n == 9) return 127;
		if (n < 9 && n>3) return 0;
		return 255;
	}

	Colour getg_oct4(int midi) const {
		int n = midi % 12;
		if (n == 1 || n == 7) return 127;
		if (n < 7 && n>1) return 255;
		return 0;
	}

	Colour getb_oct4(int midi)const {
		int n = midi % 12;
		if (n == 5 || n ==11) return 127;
		if (n < 5) return 0;
		return 255;
	}

	Colour mapoct(Colour c, int midi) {
		if (midi / 12 - 1 == 2) return conv2(c);
		if (midi / 12 - 1 == 3) return conv3(c);
		if (midi / 12 - 1 == 5) return conv5(c);
		if (midi / 12 - 1 == 6) return conv6(c);
		return c;
	}

	Colour conv2(Colour c4)const {
		return c4 - c4 / 8 * 6;
	}

	Colour conv3(Colour c4)const {
		return c4 - c4 / 8 * 3;
	}

	Colour conv5(Colour c4)const {
		return c4 + (255 - c4) / 8 * 3;
	}

	Colour conv6(Colour c4)const {
		return c4 + (255 - c4) / 8 * 6;
	}
	
};