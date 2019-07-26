#include "RPTCQ_LCD.h"

static const int Null = -1;
static const int Negative = -2;
static const int Clear = -3;




//************************* RPTCQ_DIGITS Base Class **********************//

RPTCQ_DIGITS::RPTCQ_DIGITS() {
	
}

RPTCQ_DIGITS::RPTCQ_DIGITS(Digit _ones, Digit _tens, Digit _hundreds, ST7528i _lcd) {
	ones = _ones; //{X, Y, W, H, val};
	tens = _tens; 
	hundreds = _hundreds; 
	lcd = _lcd;
	
}

RPTCQ_DIGITS::~RPTCQ_DIGITS(){

}
    
//This should take positive ints up to 999 or 
//negative ints down to -99.
void RPTCQ_DIGITS::Print(int num){
  if ((num > 999) || (num < -99)) return;
  byte i = 0;
  int a[3];
  
  if (num < 0) {
    num *= -1;
    neg = true;
  } else {
    neg = false;
  }
  while (num) {
    a[i] = (num % 10);  //modulus operator; Assign last digit
    num /= 10; //Divide by 10; Right shift number
    if (i == 0) {
      if (ones.val != a[i]){
        ones.val = a[i];
        WriteOnes(a[i]);
      }     
    }
    if (i == 1) {
      if (tens.val != a[i]){
        tens.val = a[i];
        WriteTens(a[i]);
      }     
    }
    if (!neg){
      if (i == 2) {
        if (hundreds.val != a[i]){
          hundreds.val = a[i];
          WriteHundreds(a[i]);
        }     
      }
    }      
    i++;
  } //End of while
  
  //We got a 0 ==> clear 10s and 100s positions.
  if (i == 0) {
    ClearTens ();   
    ClearHundreds(); 
	WriteOnes(0); 
  }
  //Single digit num ==> clear 10s and 100s positions.
  if (i == 1) {
    ClearTens ();   
    ClearHundreds(); 
  }
  //Double digit num ==> clear 100s position.
  if ((i == 2) && (!neg)) {
    ClearHundreds();
  }
  //If num was negative and 100s position is not set to "-" then set it.
  if (neg){
    if (hundreds.val != Negative){
      WriteHundreds(Negative);    
    }
  }
}


void RPTCQ_DIGITS::ClearTens (){
  if (tens.val != Clear) {
    lcd.ClearPartial(tens.X,tens.Y,tens.W,tens.H); //Clear 10s digit
	lcd.FlushPartial(tens.X,tens.Y,tens.W,tens.H);
    tens.val = Clear;
  }
}

void RPTCQ_DIGITS::ClearHundreds (){
  if (hundreds.val != Clear){
    lcd.ClearPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H); //Clear 100s digit
	lcd.FlushPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);
    hundreds.val = Clear; 
  }
}

//Virtual Function must be overridden.
void RPTCQ_DIGITS::WriteOnes(int num){

}
//Virtual Function must be overridden.
void RPTCQ_DIGITS::WriteTens(int num){

}
//Virtual Function must be overridden.
void RPTCQ_DIGITS::WriteHundreds(int num){

}

//*******************************RPTCQ_CHMBR Sub Class ****************************//

RPTCQ_CHMBR::~RPTCQ_CHMBR(){

}
    
void RPTCQ_CHMBR::WriteOnes(int num){
  lcd.ClearPartial(ones.X,ones.Y,ones.W,ones.H);
  lcd.DrawBitmap(ones.X,ones.Y,ones.W,ones.H,getBitmapByNumber(num));
  lcd.FlushPartial(ones.X,ones.Y,ones.W,ones.H);
}

void RPTCQ_CHMBR::WriteTens(int num){
  if (num != Clear) lcd.ClearPartial(tens.X,tens.Y,tens.W,tens.H);
  lcd.DrawBitmap(tens.X,tens.Y,tens.W,tens.H,getBitmapByNumber(num));
  lcd.FlushPartial(tens.X,tens.Y,tens.W,tens.H);
}

void RPTCQ_CHMBR::WriteHundreds(int num){
  if (num != Clear)  lcd.ClearPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);
  if (num == Negative) {
    lcd.DrawBitmap(hundreds.X,hundreds.Y,hundreds.W,hundreds.H,getBitmapByNumber(Negative));//Set 100s digit with "-"
    hundreds.val = Negative;
  } else {
    lcd.DrawBitmap(hundreds.X,hundreds.Y,hundreds.W,hundreds.H,getBitmapByNumber(num));
  }
  lcd.FlushPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);
}

const unsigned char* RPTCQ_CHMBR::getBitmapByNumber(int num){
  switch (num){
  case 0:   
    return Zero_21x33;
  case 1:       
    return One_21x33;   
  case 2:
    return Two_21x33;
  case 3:
    return Three_21x33;
  case 4:
    return Four_21x33;
  case 5:   
    return Five_21x33; 
  case 6:       
    return Six_21x33;    
  case 7:
    return Seven_21x33;
  case 8:
    return Eight_21x33;    
  case 9:
    return Nine_21x33;    
  default:
    return Minus_21x33;
  }
}


//*******************************RPTCQ_DIGWXH Sub Class ****************************//

RPTCQ_DIGWXH::RPTCQ_DIGWXH(){
//Do nothing constructor.
}
RPTCQ_DIGWXH::RPTCQ_DIGWXH(Digit _ones, Digit _tens, Digit _hundreds, ST7528i _lcd, const Font_TypeDef *_Font) :
	RPTCQ_DIGITS(_ones, _tens, _hundreds, _lcd) {

	Font = _Font;
}

RPTCQ_DIGWXH::~RPTCQ_DIGWXH(){

}

//Added the overloaded functions so we can use Font bitmaps as well as digit bitmaps.
//Do not use these for writing strings..
void RPTCQ_DIGWXH::WriteOnes(char buffer[2]){
  lcd.ClearPartial(ones.X,ones.Y,ones.W,ones.H);
  lcd.PutStr(ones.X,ones.Y,buffer,Font); 
  lcd.FlushPartial(ones.X,ones.Y,ones.W,ones.H);
}
    
void RPTCQ_DIGWXH::WriteOnes(int num){
  char buffer[2];
  itoa(num, buffer, 10);
  WriteOnes(buffer);
}

void RPTCQ_DIGWXH::WriteTens(char buffer[2]){
  if (int(buffer) != Clear) lcd.ClearPartial(tens.X,tens.Y,tens.W,tens.H);
  lcd.PutStr(tens.X,tens.Y,buffer,Font); 
  lcd.FlushPartial(tens.X,tens.Y,tens.W,tens.H);
}

void RPTCQ_DIGWXH::WriteTens(int num){
  char buffer[2];
  itoa(num, buffer, 10);
  WriteTens(buffer);
}

void RPTCQ_DIGWXH::WriteHundreds(char buffer[2]){
	if (int(buffer) != Clear)  lcd.ClearPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);	
	lcd.PutStr(hundreds.X,hundreds.Y,buffer,Font);  
	lcd.FlushPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);
}

void RPTCQ_DIGWXH::WriteHundreds(int num){ 
  if (num != Clear)  lcd.ClearPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);
  if (num == Negative) {	
	lcd.PutStr(hundreds.X,hundreds.Y,"-",Font); //Set 100s digit with "-"
	hundreds.val = Negative;
	lcd.FlushPartial(hundreds.X,hundreds.Y,hundreds.W,hundreds.H);
  } else {
	char buffer[2];
	itoa(num, buffer, 10);
	WriteHundreds(buffer);
  }
}

//*******************************RPTCQ_STRWXH Sub Class ****************************//
RPTCQ_STRWXH::RPTCQ_STRWXH(){
//Do nothing constructor.
}

// Structure describing a string of char(s)
// struct CharStr {
	// uint8_t X;
	// uint8_t Y;
	// uint8_t W;
	// uint8_t H;
	// char* str;
// };

RPTCQ_STRWXH::RPTCQ_STRWXH(CharStr _phrs, ST7528i _lcd, const Font_TypeDef *_Font){
	phrs = _phrs;
	lcd = _lcd;
	Font = _Font;
}

RPTCQ_STRWXH::~RPTCQ_STRWXH(){

}

void RPTCQ_STRWXH::Print(const char* _str){

	if (strcmp(phrs.str, _str ) == 0) return;
	lcd.ClearPartial(phrs.X,phrs.Y,phrs.W,phrs.H);
	lcd.FlushPartial(phrs.X,phrs.Y,phrs.W,phrs.H);
	//Store the returned width of the string for clearing later.
	phrs.W = lcd.PutStr(phrs.X, phrs.Y, _str, Font);  
	lcd.FlushPartial(phrs.X,phrs.Y,phrs.W,phrs.H);
	strcpy(phrs.str,_str);
}






















