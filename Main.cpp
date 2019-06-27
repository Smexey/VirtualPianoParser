#include <fstream>
#include "Parser.h"
#include "Menu.h"
#include "Measure.h"
#include "Note.h"

int main() {

	Menu meni;
	Composition * comp = nullptr;
	Parser par;
	char * defaultpathcomp = _strdup("poopfiles\\test.txt");
	char * defaultpathmap = _strdup("poopfiles\\map.csv");
	bool saved = false;
	while (true) {
		

		int choice = meni.options();
		if (choice == 1) {
			int subchoice = meni.options(1);
			if (subchoice == 1) {
				cout << "Koji path?" << endl;
				string path;
				cin >> path;
				if (path == "x") par.parsemap(defaultpathmap);
				else par.parsemap(path.c_str());
				
			}
			else if (subchoice==2) {
				cout << "Koji path?" << endl;
				string path;
				cin >> path;
				int d;
				cout << "Koji duration?(u osminama)" << endl;
				cin >> d;
				if (path == "x") comp = &par.parsecomposition(defaultpathcomp, d);
				else comp = &par.parsecomposition(path.c_str(),d);
			}
			else if (subchoice == 0) continue;
		}
		else if (choice == 2) {
			//print
			if (comp != nullptr) {
				comp->setmap(par.getmiditostr());
				cout << *comp;
			}
		}
		else if (choice == 3) {
			int subchoice = meni.options(3);
			if (subchoice == 0) continue;
			comp->changeoct(subchoice);
		}
		else if (choice == 4) {
			int subchoice = meni.options(4);
			if (subchoice == 0) continue;
			if (subchoice == 1) {
				cout << "Unesite speed multiplier:" << endl;
				int speedmult;
				cin >> speedmult;
				comp->print_midi(par.getmiditostr(), "output.mid", speedmult);
				saved = true;
			}
			else if (subchoice == 2) {
				comp->print_xml(par.getmiditostr(), "xml.txt");
				saved = true;
			}
			else if (subchoice == 3) {
				cout << "Unesite width:" << endl;
				int width;
				cin >> width;
				comp->print_bmp(width, "pic.bmp");
				saved = true;
			}

		}
		else if (choice == 0) {
			if (saved == false) {
				cout << "Exit without saving? 1/0" << endl;
				int subchoice;
				cin >> subchoice;
				if (subchoice == 1) return 0;
			}
			else return 0;
		}
	}



	//char * pathmap = _strdup("poopfiles\\map.csv");
	//Parser par2;
	//par2.parsemap(pathmap);

	//char * pathcomp = _strdup("poopfiles\\fur_elise2.txt");


	//Composition comp2 = par2.parsecomposition(pathcomp,6);

	//
	//comp2.setmap(par2.getmiditostr());
	//cout << comp2 << endl;
	//comp2.print_midi(par2.getmiditostr(),"outfiles\\output.mid",2);
	//comp2.print_xml(par2.getmiditostr(), "outfiles\\xml.txt");
	//comp2.print_bmp(15,"outfiles\\slika.bmp");


	//comp2.changeoct(+1);
	//comp2.print_xml(par2.getmiditostr(), "outfiles\\xml2.txt");
	//
	//comp2.setmap(par2.getmiditostr());
	//cout << comp2 << endl;

	////comp2.setnote(1, 2,65);
	//comp2.setmap(par2.getmiditostr());
	//cout << comp2 << endl;

	system("pause");
}