#pragma once
#include <iostream>
using namespace std;

class Menu {

public:
	Menu() {
	}

	int options(int n = 0) {
		cout << flush;
		if (n == 0) {
			cout << flush;
			cout << "MENU:" << endl;
			cout << "1.Ucitavanja" << endl;
			cout << "2.Printuj kompoziciju:" << endl;
			cout << "3.Promeni takt komp:" << endl;
			cout << "4.Printuj komp u file:" << endl;
			cout << "0.Exit" << endl;
		}
		else if (n == 1) {

			cout << "1.Ucitavanja" << endl;
			cout << "\t" << "1.Ucitaj path mape" << endl;
			cout << "\t" << "2.Ucitaj path kompozicije" << endl;
			cout << "\t" << "0.Back" << endl;
		}
		else if (n == 3) {

			cout << "3.Promeni oct komp:" << endl;
			cout << "\t" << "Unesite oct deltu:" << endl;
		
		}
		else if (n == 4) {
			cout << "4.Printuj komp u file:" << endl;
			cout << "\t" << "1.MIDI" << endl;
			cout << "\t" << "2.XML" << endl;
			cout << "\t" << "3.BMP" << endl;
			cout << "\t" << "0.Back" << endl;
		}
		int x;
		cin >> x;
		return x;

	}


};
