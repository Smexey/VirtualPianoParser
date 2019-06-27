#include "Part.h"


string Part::print_xml(Miditostr & map,string str,const Duration & beat) {
	string s;
	s += "<part id=\"" + str + "\">\n";
	s += "<measure>\n""<attributes>\n""<divisions>2</divisions>\n""<time>\n";
	s += "<beats>" + to_string(beat.getnum()) + "</beats>\n" "<beat-type>" + to_string(beat.getdenom()) + "</beat-type>\n";
	s += "</time>\n" "<clef>\n";
	if (str == "Right") {
		s+="<sign>G</sign>\n""<line>2</line>\n" "</clef>\n";
	}
	else {
		s+="<sign>F</sign>\n""<line>4</line>\n" "</clef>\n";
	}
	s += "</attributes>\n";

	bool unlink = false;
	for (Part::Iter meas = begin(); meas != end(); meas++) {
		if (meas != begin()) s += "<measure>\n";

		for (Measure::Iter notes = (**meas).begin(); notes != (**meas).end(); notes++) {
			int arg = 0;
			Measure::Iter temp = notes;
			temp++;

			if (unlink == true) {
				arg = 2;
				unlink = false;
			}
			if ((*meas)->getlinked() && temp == (**meas).end()) {
				//ako je link i poslednji
				arg = 1;
				unlink = true;//sledeci mora da se unlink
			}

			s+=(*notes)->print_xml(map, arg);
		}
		s += "</measure>\n";
	}
	s += "</part>\n";
	return s;
}

void Part::print_midi(int tpq, smf::MidiFile & outputfile) {
	vector<smf::uchar> midievent;
	midievent.resize(3);
	midievent[2] = 64;

	bool skip = false;
	int actiontime = 0;
	for (Part::Iter measures = begin(); measures != end(); measures++) {

		for (Measure::Iter notes = (**measures).begin(); notes != (**measures).end(); notes++) {
			if (skip) {
				skip = false;
				continue;
			}
			Measure::Iter temp = notes;
			temp++;
			bool merge = false;

			if (temp == (**measures).end() && (**measures).getlinked()) {
				merge = true;
				skip = true;
			}
			int midi = (*notes)->getmidi();
			if (midi == 0) {
				//pauza
				//samo se povecava action na next?
				if (!merge)actiontime += tpq / 2 * (*notes)->getdur().getdur();
				else actiontime += tpq / 2 * (*notes)->getdur().getdur() * 2;
			}
			else if (midi < 0) {
				//za chord
				int cnt = -midi;
				//pressdown
				for (size_t i = 0; i < cnt; i++) {
					Chord * ptr = dynamic_cast<Chord *>(*notes);
					int m = (*ptr)[i];

					midievent[0] = 0x90;
					midievent[1] = m;
					outputfile.addEvent(0, actiontime, midievent);

				}
				if (!merge)actiontime += tpq / 2 * (*notes)->getdur().getdur();
				else actiontime += tpq / 2 * (*notes)->getdur().getdur() * 2;

				//release in next takt
				for (size_t i = 0; i < cnt; i++) {
					Chord * ptr = dynamic_cast<Chord *>(*notes);
					int m = (*ptr)[i];

					midievent[0] = 0x80;
					midievent[1] = m;
					outputfile.addEvent(0, actiontime, midievent);
				}

			}
			else {
				//regularna nota
				midievent[0] = 0x90;
				midievent[1] = midi;

				outputfile.addEvent(0, actiontime, midievent);

				if (!merge)actiontime += tpq / 2 * (*notes)->getdur().getdur();
				else actiontime += tpq / 2 * (*notes)->getdur().getdur() * 2;

				midievent[0] = 0x80;
				outputfile.addEvent(0, actiontime, midievent);
			}

		}

	}
}