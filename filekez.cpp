#include "stdafx.h"
#include "filekez.h"

//Filekez *FilekezObj;

//Filekez class ?s egyedi f?jlkezel?s by Szinger Henrik -- a Ledmatrixhoz lett meg?rva, majd ?toll?zva az NGS_LEDS-hez, el?g specifikus, nem javasolt m?shova felhaszn?lni
//a Cngdledapp initj?ben t?rt?nik egy Filekez::Get();, az eg?sz class ?gynevezett Singleton-k?nt van meg?rva, magyarul olyan class aminek csak egy p?ld?nya van
// a Ledm?trixhoz a singleton j? megold?snak t?nt, az NGS_LEDShez m?r kev?sb?, hiszen lehetne k?t p?ld?nyom egyik a gombok egyik pedig a ledekre, gyorsabb volt kieg?sz?teni, b?r vesztett az olvashat?s?gb?l
//Alapj?raton ?rdemes el?re meg?rt ini parsert haszn?lni, nekem belef?rt hogy meg?rjak egy potato saj?tot, az NGS_LEDS projektre meg m?r haszn?ltam volna rendeset, csak az 64 bites programhoz k?sz?lt,
// a Keilhez pedig nem most akartam neki?llni k?s?rletezni ezzel


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

							if ((portnum >= '0' && portnum <= '4') && (bitnum >= '0' && bitnum <= '7')) { // minim?lis sz?r?s hogy adott korl?ton bel?l van-e a configolt ?rt?k
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

//a beolvasott "lok?lisan" t?rolt ?rt?keket ?tteszi Ext v?ltoz?kba amik el?rhet?ek m?s classb?l is
std::array<std::bitset<8>, 5> Filekez::portExt = Filekez::Get().port;  
std::array<uint8_t, 5> Filekez::sfraddress = { 0x80,0x90,0xA0,0xB0,0xC0 };
std::array<uint8_t, 16> Filekez::portArrayExt = Filekez::Get().portarray;	// a portArrayExt tartalmazza hogy pl az els? ledhez a 3mas port lett konfigolva
std::array<uint8_t, 16> Filekez::bitArrayExt = Filekez::Get().bitarray;		// a bitArrayExt tartalmazza hogy pl az els? ledhez az 5?s bit lett konfigolva
																			// teh?t a k?s?bbiekben egy currPort[Filekez::portArrayExt[1]][Filekez::bitArrayExt[1]], ?gy j?n ki, hogy currport[3][5]