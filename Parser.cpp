#include "Parser.h"

Notesmap& Parser::parsemap(const char * path) {
	//pravi dve mape:
	//notesmap char[iz virtualpiana]->pair<string[ime note], int[midi val]>
	//miditos int[midi val]->string[ime note]
	//razlog je krpljenje koda
	nmap = new Notesmap();
	miditos = new Miditostr();
	ifstream file;
	file.open(path, ios::in);

	if (!file.is_open()) {
		throw Greska("Notesmap file not opened correctly");
	}

	string line;
	int i = 0;
	while (file >> line) {
		smatch matches;
		regex re("([^,]+),([^,]+),([^,]+)");
		regex_search(line, matches, re);
		if (matches.size() != 4) {
			file.close();
			throw Greska(string("Notesmap file bad line at: ") + to_string(i));
		}
		pair<string, int> val(matches.str(2), stoi(matches.str(3)));
		pair<char, pair<string, int>> p(matches.str(1).c_str()[0], val);
		nmap->insert(p);
		pair<int, string> p2(stoi(matches.str(3)), matches.str(2));
		miditos->insert(p2);

		i++;
	}
	file.close();
	return *nmap;
}

Composition& Parser::parsecomposition(const char * path, const Duration & d) {
	//pps nema gresaka u fajlu
	//moguce da char iz fajla ne bude u mapi tada se throwuje nesto a mapa vraca null?

	int oct3 = 59;///poslednja u okt 3

	ifstream file;
	file.open(path, ios::in);
	Composition &comp = *new Composition(d);

	if (!file.is_open()) {
		throw Greska("Composition not opened correctly");
	}

	Measure * measmelody = new Measure(d);
	Measure * measbass = new Measure(d);

	string line;
	while (std::getline(file, line)) {

		smatch res;
		string::const_iterator searchStart(line.cbegin());
		regex exp("(\\[[^\\]]*\\]|[^\\[])");

		while (regex_search(searchStart, line.cend(), res, exp))
		{
			string str = res.str(0);
			if (str.length() == 1) {
				//nije ni chord ni uzastopni
				if (str == " ") {
					measmelody->insert(new Pause(1));
					measbass->insert(new Pause(1));
				}
				else if (str == "|") {
					if (measmelody->getdur() + 2 > measmelody->getmaxdur()) {
						//ako je vece od max split
						measmelody->insert(new Pause(1));
						measbass->insert(new Pause(1));
						measmelody->setlinked(true);
						measbass->setlinked(true);

						pushtocomp(comp, measmelody, measbass, d);

						measmelody->insert(new Pause(1));
						measbass->insert(new Pause(1));
					}
					else {
						//ako upada samo se ubaci
						measmelody->insert(new Pause(2));
						measbass->insert(new Pause(2));
					}
				}
				else {
					//regularna nota
					if ((*nmap)[str[0]].second > oct3) insertregularnote(measbass, measmelody, str, comp, d,2);
					else insertregularnote(measmelody, measbass, str, comp, d,1);
				}
			}
			else {
				//chord ili uzastopni

				if (str[2] != ' ') {
					//chord
					int * arrbass = new int[str.length() - 2];
					int * arrmelo = new int[str.length() - 2];
					int basscnt = 0;
					int melocnt = 0;

					for (size_t i = 1; i < str.length() - 1; i++) {

						if ((*nmap)[str[i]].second > oct3) {
							//ide u melo
							arrmelo[melocnt++] = (*nmap)[str[i]].second;
						}
						else {
							arrbass[basscnt++] = (*nmap)[str[i]].second;
						}
					}
					if (measmelody->getdur() + 2 > measmelody->getmaxdur()) {
						//ako je vece od max split

						if (melocnt > 0) measmelody->insert(new Chord(arrmelo, melocnt, 1));
						else {
							delete[] arrmelo;
							measmelody->insert(new Pause(1));
						}
						if (basscnt > 0) measbass->insert(new Chord(arrbass, basscnt, 1));
						else {
							delete[] arrbass;
							measbass->insert(new Pause(1));
						}
						int * arrmelocpy = new int[melocnt];
						for (size_t i = 0; i < melocnt; i++) arrmelocpy[i] = arrmelo[i];
						
						int * arrbasscpy = new int[basscnt];
						for (size_t i = 0; i < basscnt; i++)arrbasscpy[i] = arrbass[i];

						measmelody->setlinked(true);
						measbass->setlinked(true);

						pushtocomp(comp, measmelody, measbass, d);

						if (melocnt > 0) measmelody->insert(new Chord(arrmelocpy, melocnt, 1));
						else {
							delete[] arrmelocpy;
							measmelody->insert(new Pause(1));
						}

						if (basscnt > 0) measbass->insert(new Chord(arrbasscpy, basscnt, 1));
						else {
							delete[] arrbasscpy;
							measbass->insert(new Pause(1));
						}
					}
					else {
						//ako upada samo se ubaci
						if(melocnt>0) measmelody->insert(new Chord(arrmelo, melocnt, 2));
						else measmelody->insert(new Pause(2));

						if (basscnt > 0) measbass->insert(new Chord(arrbass, basscnt, 2));
						else measbass->insert(new Pause(2));
					}
				}
				else {
					//uzastopni
					for (size_t i = 1; i < str.length() - 1; i += 2) {
						//za svaki elem u ovom njesra   //i se povecava za 2
						MusicSymbol * m = new Note((*nmap)[str[i]].second, 1);
						MusicSymbol * p = new Pause(1);
						if ((*nmap)[str[i]].second > oct3) {
							//melody ubacujem
							measmelody->insert(m);
							measbass->insert(p);
						}
						else {
							//bass ubacujem
							measmelody->insert(p);
							measbass->insert(m);
						}
						//proveri punost
						if (measbass->getdur() == measbass->getmaxdur()) {
							pushtocomp(comp, measmelody, measbass, d);
						}
					}
				}
			}
			//nakon ubacivanja alociram nove ako su puni
			if (measbass->getdur() == measbass->getmaxdur()) {
				pushtocomp(comp, measmelody, measbass, d);
			}
			searchStart = res.suffix().first;
		}

	}
	file.close();
	
	while (measbass->getdur() < measbass->getmaxdur() && measbass->getdur()!=0) {
		measmelody->insert(new Pause(1));
		measbass->insert(new Pause(1));
	}

	if (measbass->getdur() > 0) {
		pushtocomp(comp, measmelody, measbass, d);
	}
	delete measbass;
	delete measmelody;

	return comp;
}

void Parser::pushtocomp(Composition & comp, Measure * & measmelody, Measure*& measbass, const Duration & d, int mode) {
	if (mode == 1) {
		comp.insertmelody(measmelody);
		comp.insertbass(measbass);
	}
	else {
		comp.insertmelody(measbass);
		comp.insertbass(measmelody);
	}
	measmelody = new Measure(d);
	measbass = new Measure(d);
}

void Parser::insertregularnote(Measure*& measmelody, Measure *& measbass, string& str, Composition & comp, const Duration& d, int mode ) {
	//mode 1 je za bass(regularni)
	//leva ruka odnosno bass
	if (measmelody->getdur() + 2 > measmelody->getmaxdur()) {
		//ako je vece od max split
		MusicSymbol * m1 = new Note((*nmap)[str[0]].second, 1);
		measbass->insert(m1);
		measmelody->insert(new Pause(1));
		measmelody->setlinked(true);
		measbass->setlinked(true);

		pushtocomp(comp, measmelody, measbass, d, mode);

		m1 = new Note((*nmap)[str[0]].second, 1);
		measbass->insert(m1);
		measmelody->insert(new Pause(1));
	}
	else {
		//ako upada samo se ubaci
		MusicSymbol * m = new Note((*nmap)[str[0]].second, 2);
		measbass->insert(m);
		measmelody->insert(new Pause(2));
	}
}