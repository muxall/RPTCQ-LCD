//This is an example Arduino Sketch Main program.

#include "st7528i.h"
#include "RPTCQ_LCD.h"

// Resources
#include "font5x7.h"
#include "font7x10.h"
//#include "Bitmaps.h"
#include "digits5x9.h"
#include "digits8x16.h"
#include "logos.h"

//LCD GPIO/Pin settings.
#define SDA1 0
#define SCL1 4
#define LCD_RST 2
#define BKLIGHT_PIN 9

char tempScale[2] = "F";
char LcdFsmStateL1[5] = "BBQ";
char LcdFsmStateL2[5] = "OFF";

//LCD Global object placeholder.
ST7528i lcd;

void setup() {

	//LCD pin init
	  pinMode(BKLIGHT_PIN, OUTPUT);
	  digitalWrite(BKLIGHT_PIN,LOW);
	  pinMode(LCD_RST, OUTPUT);
	  digitalWrite(LCD_RST,LOW);
	  delay(200); //Delay for LCD reset. 
	  
		//LCD Setup
	  Serial.print("\nBooting LCD... ");
	  digitalWrite(LCD_RST, HIGH);// 
	  delay(500);  
	  init_LCD();
	  Serial.println("Done!");
	
	  //LCD Init Status
	  LcdInitStatus();
	  LcdDrawStatus();
}

void loop(){
	
	LcdPrintStatus();
	delay(2000);
	
	strcpy(LcdFsmStateL1,"BBQ");
    strcpy(LcdFsmStateL2,"ON");
	
}














