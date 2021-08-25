#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <array>
#include "WinUser.h"


class Filekez 
{
public:
	Filekez(const Filekez&) = delete;
	static Filekez& Get()
	{
		static Filekez fileobj;
		return fileobj;
	}

	static std::array<std::bitset<8>, 5> portExt;
	static std::array<uint8_t, 5> sfraddress; // Sfr adresses for P0-P4
	
	static std::array<uint8_t, 16> portArrayExt;  // port for the idfier under the same index
	static std::array<uint8_t, 16> bitArrayExt;	// bit for the idfier under the same index

	std::array<uint8_t, 16> gievBackPort() {
		return portarray;
	}

private:
	Filekez();
	//static Filekez* fobj();
	std::array<std::string, 16> idfier = { "LED1","LED2","LED3","LED4","LED5","LED6","LED7","LED8","BTN1","BTN2","BTN3","BTN4","BTN5","BTN6","BTN7","BTN8" };
	std::array<std::bitset<8>, 5> port;
	std::array<uint8_t, 16> portarray;  // port for the idfier under the same index
	std::array<uint8_t, 16> bitarray;	// bit for the idfier under the same index

};


//extern Filekez* FilekezObj;