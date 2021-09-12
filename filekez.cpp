#include "stdafx.h"
#include "filekez.h"

//Filekez *FilekezObj;

//Filekez class és egyedi fájlkezelés by Szinger Henrik -- a Ledmatrixhoz lett megírva, majd átollózva az NGS_LEDS-hez, elég specifikus, nem javasolt máshova felhasználni
//a Cngdledapp initjében történik egy Filekez::Get();, az egész class úgynevezett Singleton-ként van megírva, magyarul olyan class aminek csak egy példánya van
// a Ledmátrixhoz a singleton jó megoldásnak tûnt, az NGS_LEDShez már kevésbé, hiszen lehetne két példányom egyik a gombok egyik pedig a ledekre, gyorsabb volt kiegészíteni, bár vesztett az olvashatóságból
//Alapjáraton érdemes elõre megírt ini parsert használni, nekem belefért hogy megírjak egy potato sajátot, az NGS_LEDS projektre meg már használtam volna rendeset, csak az 64 bites programhoz készült,
// a Keilhez pedig nem most akartam nekiállni kísérletezni ezzel


Filekez::Filekez()
{

	//FilekezObj = this;
	//std::filesystem::path FilekezFilepath = CLedmatrixApp::FilekezFilepath;  //namespace requires C++17
	//std::string path = FilekezFilepath.u8string()+"\\Ledmatrix.ini";

	std::ifstream fileread("NGS_LEDS.ini"); 
	//std::ifstream fileread(path);		### Uncomment lines 12,13,16 if filekez stuff gets called later than Ledmatrix.cpp ?
	if (!fileread) {
		MessageBox(nullptr, TEXT("NGS_LEDS.ini not found"), TEXT("Message"), MB_OK);
		return;
	}
	std::string line;
	uint8_t portnum;
	uint8_t bitnum;
	size_t found;
	
	while (getline(fileread, line)) {		//read until the end of file

		for (char x = 0; x < 16; x++) {		
			if (line.find(idfier[x]) != std::string::npos) { //look for the control names
				if ((found = line.find("=")) != std::string::npos && (line.length() >= found + 4)) { 
					try {		
						if ((line.at(found + 1) == 'P') &&
							(line.at(found + 3) == '.'))
						{
							portnum = line.at(found + 2);
							bitnum = line.at(found + 4);

							if ((portnum >= '0' && portnum <= '4') && (bitnum >= '0' && bitnum <= '7')) { // minimális szûrés hogy adott korláton belül van-e a configolt érték
								/*if (port[(portnum - '0')][(bitnum - '0')] == 0) {*/
									portnum -= '0';
									bitnum -= '0';
									port[portnum].set(bitnum);
									portarray[x] = portnum;
									bitarray[x] = bitnum;  // Exception thrown if last line is like "P4. "
								/*}*/
							}
						}
					}
					catch (std::out_of_range) { // (std::out_of_range const& exc)
						MessageBox(nullptr, TEXT("Invalid port/bit definition in file NGS_LEDS.ini"), TEXT("Invalid definition"), MB_OK);
					}
				}
			}
		}
	}
	
	fileread.close();
}

//a beolvasott "lokálisan" tárolt értékeket átteszi Ext változókba amik elérhetõek más classból is
std::array<std::bitset<8>, 5> Filekez::portExt = Filekez::Get().port;  
std::array<uint8_t, 5> Filekez::sfraddress = { 0x80,0x90,0xA0,0xB0,0xC0 };
std::array<uint8_t, 16> Filekez::portArrayExt = Filekez::Get().portarray;	// a portArrayExt tartalmazza hogy pl az elsõ ledhez a 3mas port lett konfigolva
std::array<uint8_t, 16> Filekez::bitArrayExt = Filekez::Get().bitarray;		// a bitArrayExt tartalmazza hogy pl az elsõ ledhez az 5ös bit lett konfigolva
																			// tehát a késõbbiekben egy currPort[Filekez::portArrayExt[1]][Filekez::bitArrayExt[1]], úgy jön ki, hogy currport[3][5]