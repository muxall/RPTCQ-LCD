#ifndef RPTCQ_LCD_h
#define RPTCQ_LCD_h

#include "st7528i.h"
#include "Bitmaps.h"
#include "digits8x16.h"

// Structure describing a digit
struct Digit {
	uint8_t X;
	uint8_t Y;
	uint8_t W;
	uint8_t H;
	int val;
};

// Structure describing a string of char(s)
struct CharStr {
	uint8_t X;
	uint8_t Y;
	uint8_t W;
	uint8_t H;
	char str[25];
};

//************************* RPTCQ_DIGITS Base Class **********************//
/*
Override WriteOnes, Tens, Hundereds.
*/
class RPTCQ_DIGITS {
	
public:
	bool neg;
	Digit ones;
	Digit tens;
	Digit hundreds;

	ST7528i lcd;
	//Constructors
		RPTCQ_DIGITS();
		RPTCQ_DIGITS(Digit _ones, Digit _tens, Digit _hundreds, ST7528i _lcd);
		~RPTCQ_DIGITS();
	
	//Function Prototypes
	void ClearTens();
	void ClearHundreds();
	void Print(int temp);
	virtual void WriteOnes(int num);
	virtual void WriteTens(int num);
	virtual void WriteHundreds(int num);	
};

//*******************************RPTCQ_CHMBR Sub Class ****************************//
/* This is for the 21x33 Bitmaps that we use for the chamber temp.
Examples:
RPTCQ_CHMBR LcdChamber;  //LCD ChamberTemp
// Subtracted -2 from height because of junk in bmp.
Digit ct_hundreds = {50,2,21,30, -1};//{X, Y, W, H, val}; val= -1 is Null;
Digit ct_tens = {73,2,21,30, -1};
Digit ct_ones = {96,2,21,30, -1};
LcdChamber = RPTCQ_CHMBR(ct_ones, ct_tens, ct_hundreds, lcd); //The 21x33 is hard-coded
LcdChamber.Print(chamberTemp); 
   
Library Ex. lcd.DrawBitmap(50,2,21,33,One_21x33);   //DrawBitmap(X, Y, W, H, pBitmap);

*/
class RPTCQ_CHMBR : public  RPTCQ_DIGITS{


private:
	
	//Private functions
	const unsigned char* getBitmapByNumber(int num);

public:
	//Constructors
		RPTCQ_CHMBR() :
			RPTCQ_DIGITS(){};
		RPTCQ_CHMBR(Digit _ones, Digit _tens, Digit _hundreds, ST7528i _lcd) :
			RPTCQ_DIGITS(_ones, _tens, _hundreds, _lcd){};
		~RPTCQ_CHMBR();
	
	//Function Prototypes
	//void Print(int temp);
	void WriteOnes(int num);
	void WriteTens(int num);
	void WriteHundreds(int num);	
	
};

//*******************************RPTCQ_DIGWXH Sub Class ****************************//
/* This is for Digits only.  It uses Font and Digit ascii bitmaps.  
All digits except chamber temp.

Examples:
RPTCQ_DIGWXH LcdTargetProbeTemp;  //LCD Target Meat Probe Temp
//{X, Y, W, H, val}; Add +1 pixel to X for spacing between ones, tens, etc.
Digit tpt_hundreds = {97,83,8,16, -1};
Digit tpt_tens = {106,83,8,16, -1};
Digit tpt_ones = {115,83,8,16, -1};
   
Library Ex. //lcd.PutInt(97,83,145,dig8x16); // PutInt( X, Y, num, *Font);

*/
class RPTCQ_DIGWXH : public  RPTCQ_DIGITS{

public:
	//Constructors
		RPTCQ_DIGWXH();
		RPTCQ_DIGWXH(Digit _ones, Digit _tens, Digit _hundreds, ST7528i _lcd, const Font_TypeDef *_Font);
		~RPTCQ_DIGWXH();
	
	const Font_TypeDef *Font;
	
	//Function Prototypes
	void WriteOnes(char buffer[2]);
	void WriteOnes(int num);
	void WriteTens(char buffer[2]);
	void WriteTens(int num);
	void WriteHundreds(char buffer[2]);
	void WriteHundreds(int num);	
	
};

//************************* RPTCQ_STRWXH Base Class **********************//
/*
This is for Fonts for Strings that we use.

Examples:
RPTCQ_STRWXH LcdFsmLabel1;	//Instantiate
CharStr fsm1_str = {124,42,7,9,""};	//Initialize CharStr struct.
LcdFsmLabel1 = RPTCQ_STRWXH(fsm1_str, lcd, fnt7x10);	//Init Class
LcdFsmLabel1.Print(LcdFsmStateL1);	//Use
   
Library Example. 
//lcd.PutStr(124,45,LcdFsmStateL1,fnt7x10);  //PutStr( X, Y, str, *Font);
*/
class RPTCQ_STRWXH {
	
public:
	CharStr phrs;
	ST7528i lcd;
	const Font_TypeDef *Font;
	
	//Constructors
	RPTCQ_STRWXH();
	RPTCQ_STRWXH(CharStr _phrs, ST7528i _lcd, const Font_TypeDef *_Font);
	~RPTCQ_STRWXH();
			
	//Function Prototypes
	void Print(const char* str);
		
	
};


#endif // RPTCQ_LCD_h