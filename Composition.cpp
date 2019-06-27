#include "Composition.h"
#include "MidiFile.h"
#include "Chord.h"
#include "Note.h"
#include "Pause.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace smf;

void Composition::setnote(int measureind, int noteind, int midi) {
	int dur = (*(bass[measureind]))[noteind]->getdur().getdur();
	delete (*(bass[measureind]))[noteind];
	delete (*(melody[measureind]))[noteind];

	while (midi < 12 * 3) midi += 12;
	while (midi >= 12 * 6) midi -= 12;

	if (midi >= 4 * 12) {
		(*(melody[measureind]))[noteind] = new Note(midi, dur);
		(*(bass[measureind]))[noteind] = new Pause(dur);
	}
	else {
		(*(bass[measureind]))[noteind] = new Note(midi, dur);
		(*(melody[measureind]))[noteind] = new Pause(dur);
	}
}

Composition& Composition::print_xml(Miditostr & map, string outfilename) {
	ofstream file;
	file.open(outfilename, ios::out);

	file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl << "<!DOCTYPE score-partwise PUBLIC" << endl;
	file << "\"-//Recordare//DTD MusicXML 3.1 Partwise//EN\"" << endl << "\"http://www.musicxml.org/dtds/partwise.dtd\">" << endl;
	file << "<score-partwise version=\"3.1\">" << endl << "<part-list>" << endl << "<score-part id=\"Right\">" << "</score-part>" << endl;
	file << "<score-part id=\"Left\">" << "</score-part>" << endl << "</part-list>" << endl;


	file << melody.print_xml(map, "Right", beat);
	file << bass.print_xml(map, "Left", beat);
	file << "</score-partwise>";

	file.close();
	return *this;
}

Composition& Composition::print_midi(Miditostr & map, string outfilename, int speed) {
	MidiFile outputfile;
	outputfile.absoluteTicks();
	int tpq = 48;
	outputfile.setTicksPerQuarterNote(tpq);
	tpq /= speed;
	outputfile.addTrack(1);

	melody.print_midi(tpq, outputfile);
	bass.print_midi(tpq, outputfile);

	outputfile.sortTracks();
	outputfile.write(outfilename);
	return *this;
}

Composition& Composition::print_bmp(int w, string outfilename) {

	std::vector<Colour*> arr;
	//napravi vektor boja
	int cnt = 0;
	int height = 0;
	Part::Iter bassmeas = bass.begin();
	for (Part::Iter meas = melody.begin(); meas != melody.end(); meas++, bassmeas++) {
		Measure::Iter bassnotes = (**bassmeas).begin();
		for (Measure::Iter notes = (**meas).begin(); notes != (**meas).end(); notes++, bassnotes++) {
			for (size_t i = 0; i < (*notes)->getdur().getdur(); i++) {
				Colour* c = new Colour[3];
				int midi = (*notes)->getmidi();
				int bassmidi = (*bassnotes)->getmidi();

				//chordovi
				if (midi < 0 || bassmidi < 0) {
					for (size_t i = 0; i < 3; i++) c[i] = 0;
					Chord * ptr = dynamic_cast<Chord*>(*notes);
					size_t j1 = 0;
					for (; j1 < -midi; j1++) {
						int midic = (*ptr)[j1];
						c[0] += getb(midic);
						c[1] += getg(midic);
						c[2] += getr(midic);
					}
					ptr = dynamic_cast<Chord*>(*bassnotes);
					size_t j2 = 0;
					for (; j2 < -bassmidi; j2++) {
						int midic = (*ptr)[j2];
						c[0] += getb(midic);
						c[1] += getg(midic);
						c[2] += getr(midic);
					}
					c[0] /= j1 + j2;
					c[1] /= j1 + j2;
					c[2] /= j1 + j2;
				}
				else {
					if (midi != 0) {
						c[0] = getb(midi);
						c[1] = getg(midi);
						c[2] = getr(midi);
					}
					else {
						c[0] = getb(bassmidi);
						c[1] = getg(bassmidi);
						c[2] = getr(bassmidi);
					}
					//BGR obrnuti rgb
				}
				arr.push_back(c);
				cnt++;
				if (cnt == w) height++;
				cnt %= w;
			}

		}

	}

	if (cnt < w) {
		//paddovanje bele
		for (; cnt < w; cnt++) {
			Colour* c = new Colour[3];
			c[0] = 255;
			c[1] = 255;
			c[2] = 255;
			arr.push_back(c);
		}
		height++;
	}
	ofstream file;
	file.open(outfilename, ios::out | ios::binary);
	typedef unsigned short ushort;
	ushort a = 0;
	unsigned b = 0;
	a = 0x42;
	file.write(reinterpret_cast<const char*>(&a), 1);
	a = (ushort)0x4D;
	file.write(reinterpret_cast<const char*>(&a), 1);

	//size kod
	unsigned size = (w&(~(7)));
	if (w & 7) {
		size += 8;
	}
	int bytestofullrow = size - w;

	size *= arr.size();


	b = (size + 16);
	file.write(reinterpret_cast<const char*>(&b), 4);

	b = (unsigned int)0;
	file.write(reinterpret_cast<const char*>(&b), 4);
	b = 0x36;
	file.write(reinterpret_cast<const char*>(&b), 4);
	b = 0x28;
	file.write(reinterpret_cast<const char*>(&b), 4);
	//width
	b = (w);
	file.write(reinterpret_cast<const char*>(&b), 4);
	//height
	b = (height);
	file.write(reinterpret_cast<const char*>(&b), 4);

	a = (ushort)1;
	file.write(reinterpret_cast<const char*>(&a), 1);
	a = (ushort)0;
	file.write(reinterpret_cast<const char*>(&a), 1);
	a = (ushort)0x18;
	file.write(reinterpret_cast<const char*>(&a), 1);
	a = (ushort)0;
	file.write(reinterpret_cast<const char*>(&a), 1);
	b = (unsigned)0;
	file.write(reinterpret_cast<const char*>(&b), 4);

	//size
	b = (size);
	file.write(reinterpret_cast<const char*>(&b), 4);
	b = 0x13 | (0x0B00);
	file.write(reinterpret_cast<const char*>(&b), 4);
	file.write(reinterpret_cast<const char*>(&b), 4);
	b = (unsigned)0;
	file.write(reinterpret_cast<const char*>(&b), 4);
	file.write(reinterpret_cast<const char*>(&b), 4);

	//COLOURSSSSSSS
	int i = 0;
	int numbytes = 0;
	int y = arr.size() / w - 1;
	while (y >= 0) {

		file.write(reinterpret_cast<const char*>(&arr[y*w + i][0]), 1);
		file.write(reinterpret_cast<const char*>(&arr[y*w + i][1]), 1);
		file.write(reinterpret_cast<const char*>(&arr[y*w + i][2]), 1);
		i++;
		numbytes += 3;

		if (i == w) {
			//dodaj bele
			int x;
			x = (numbytes&(~(3)));
			if (w & 3) {
				x += 4;
			}
			x -= numbytes;
			for (size_t j = 0; j < x; j++) {
				ushort c = 0;
				file.write(reinterpret_cast<const char*>(&c), 1);
			}
			numbytes = 0;
			i = 0;
			y--;
		}
	}

	file.close();
	return *this;
}

Composition & Composition::changeoct(int delta) {
	for (Part::Iter measures = bass.begin(); measures != bass.end(); measures++) {
		for (Measure::Iter notes = (**measures).begin(); notes != (**measures).end(); notes++);
	}

	Part::Iter measurebass = bass.begin();
	Part::Iter measuremelo = melody.begin();
	while (measurebass != bass.end()) {
		Measure::Iter notebass = (**measurebass).begin();
		Measure::Iter notemelo = (**measuremelo).begin();
		while (notebass != (**measurebass).end()) {
			//oba imaju chord
			if ((*notebass)->getmidi() < 0 || (*notemelo)->getmidi() < 0) {
				int max = 0;
				if ((*notebass)->getmidi() < 0)max += -((*notebass)->getmidi());
				if ((*notemelo)->getmidi() < 0)max += -((*notemelo)->getmidi());

				int * arrbass = new int[max];
				int * arrmelo = new int[max];
				int basscnt = 0;
				int melocnt = 0;
				int i = 0;
				Chord & chr = *dynamic_cast<Chord*>(*notebass);
				while (i < -(*notebass)->getmidi()) {
					int newmidi = chr[i] + 12 * delta;
					while (newmidi < 12 * 3)newmidi += 12;
					while (newmidi >= 12 * 6) newmidi -= 12;
					if (newmidi >= 4 * 12) {
						arrmelo[melocnt++] = newmidi;
					}
					else {
						arrbass[basscnt++] = newmidi;
					}
					i++;
				}
				i = 0;
				Chord & chr2 = *dynamic_cast<Chord*>(*notemelo);
				while (i < -(*notemelo)->getmidi()) {
					int newmidi = chr2[i] + 12 * delta;
					while (newmidi < 12 * 3)newmidi += 12;
					while (newmidi >= 12 * 6) newmidi -= 12;
					if (newmidi >= 4 * 12) {
						arrmelo[melocnt++] = newmidi;
					}
					else {
						arrbass[basscnt++] = newmidi;
					}
					i++;
				}
				MusicSymbol * old = (*notebass);
				if (basscnt > 0) (*notebass) = new Chord(arrbass, basscnt, (*notebass)->getdur());
				else (*notebass) = new Pause((*notebass)->getdur());

				delete old;
				old = (*notemelo);
				if (melocnt > 0) (*notemelo) = new Chord(arrmelo, melocnt, (*notemelo)->getdur());
				else (*notemelo) = new Pause((*notebass)->getdur());
				delete old;
			}
			else if ((*notebass)->getmidi() != 0) {
				//bass ima notu
				int newmidi = ((*notebass)->getmidi() + 12 * delta);
				while (newmidi < 12 * 3)newmidi += 12;
				while (newmidi >= 12 * 6) newmidi -= 12;
				(*notebass)->setmidi(newmidi);
				if (newmidi >= 4 * 12) {
					//swap
					MusicSymbol * temp = (*notebass);
					(*notebass) = (*notemelo);
					(*notemelo) = temp;
				}
			}
			else if ((*notemelo)->getmidi() != 0) {
				//melo ima notu
				int newmidi = ((*notemelo)->getmidi() + 12 * delta);
				while (newmidi < 12 * 3)newmidi += 12;
				while (newmidi >= 12 * 6) newmidi -= 12;
				(*notemelo)->setmidi(newmidi);
				if (newmidi < 4 * 12) {
					//swap
					MusicSymbol * temp = (*notebass);
					(*notebass) = (*notemelo);
					(*notemelo) = temp;
				}
			}


			notebass++;
			notemelo++;
		}
		measurebass++;
		measuremelo++;
	}

	return *this;
}
