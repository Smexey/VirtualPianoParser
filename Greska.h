#pragma once
#include <iostream>
#include <exception>


class Greska :public std::exception {
	std::string msg;

public:
	explicit Greska(const std::string& message) :msg(message) {
	}
	explicit Greska(const char* message) :msg(message) {
	}

	virtual char const * what() const { 
		return msg.c_str(); 
	}

};