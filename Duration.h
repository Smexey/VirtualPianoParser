#pragma once
#include "Greska.h"

class Duration {
	static enum Fractal {
		QUARTER = 4, EIGHT = 8
	};
	std::pair<int, int> fract;
	int dura;
public:
	
	Duration(std::pair<int,int> p):fract(p) {
		if (p.second != QUARTER && p.second != EIGHT) {
			throw Greska("Unsupported duration");
		}
		dura = p.first;
		if (p.second == QUARTER) {
			dura *= 2;
		}

	}

	Duration(int n=1) {
		dura = n;
		int num = n;
		int denom = 8;
		if ((num & 1) == 0) {
			num /= 2;
			denom /= 2;
		}
		std::pair<int, int> p(num, denom);
		fract = p;
	}

	int getdur() const {
		return dura;
	}

	Duration & setdur(int n) {
		dura = n;
		int num = n;
		int denom = 8;
		if ((num & 1) == 0) {
			num /= 2;
			denom /= 2;
		}
		std::pair<int, int> p(num, denom);
		fract = p;
	}

	const std::pair<int, int>& getdurafract() const {
		return fract;
	}

	int getnum()const {
		return fract.first;
	}

	int getdenom()const {
		return fract.second;
	}

	friend std::ostream& operator<<(std::ostream& os, const Duration& d) {
		os << d.fract.first << "/" << d.fract.second;
		return os;
	}

};