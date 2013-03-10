// blowfish.h     interface file for blowfish.cpp
// _THE BLOWFISH ENCRYPTION ALGORITHM_
// by Bruce Schneier
// Revised code--3/20/94
// Converted to C++ class 5/96, Jim Conger
//
// modify H.Shirouzu 07/2002 (add change_order(), CBC mode)

#define MAXKEYBYTES		56		// 448 bits max
#define NPASS			16		// SBox passes

#define DWORD	unsigned long
#define WORD	unsigned short
#define BYTE	unsigned char

class CBlowFish
{
public:
	enum Mode { ECB, CBC /*, OFB, CFB */ };

private:
	DWORD	*PArray;
	DWORD	(*SBoxes)[256];
	Mode	mode;
	void	Blowfish_encipher(DWORD *xl, DWORD *xr);
	void	Blowfish_decipher(DWORD *xl, DWORD *xr);

public:
			CBlowFish();
			~CBlowFish();
	void	Initialize(BYTE key[], int keybytes, Mode _mode=ECB);
	DWORD	GetOutputLength(DWORD lInputLong);
	DWORD	Encode(BYTE * pInput, BYTE * pOutput, DWORD lSize);
	DWORD	Decode(BYTE * pInput, BYTE * pOutput, DWORD lSize);
};

// choose a byte order for your hardware
#define ORDER_DCBA	// chosing Intel in this case

union aword {
	DWORD	dword;
	BYTE	byte[4];
	struct {
#ifdef ORDER_ABCD  	// ABCD - big endian - motorola
		BYTE byte0;
		BYTE byte1;
		BYTE byte2;
		BYTE byte3;
#endif
#ifdef ORDER_DCBA  	// DCBA - little endian - intel
		BYTE byte3;
		BYTE byte2;
		BYTE byte1;
		BYTE byte0;
#endif
#ifdef ORDER_BADC  	// BADC - vax
		BYTE byte1;
		BYTE byte0;
		BYTE byte3;
		BYTE byte2;
#endif
	} w;
};


