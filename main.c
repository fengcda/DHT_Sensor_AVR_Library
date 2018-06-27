/*
 * Example test bench for DHT22 on an Atmega 1284 
 *
 * Author      : David Feng
 * Updated     : June 26th, 2018
 * Usage       : Compiled on AVR-GCC using Atmel Studio 7 
 */

#ifndef F_CPU
#define F_CPU 8000000UL //define microcontroller clock speed
#endif

#include <util/delay.h>
#include "lcd_io.c" //library that controls LCD display
#include "DHT.c" //library that works with DHT sensor

uint16_t temperature_int = 0;
uint16_t humidity_int = 0;

//function to convert number values into characters
void LCD_print(uint16_t temp, unsigned char cur_count) {
	
	unsigned char ten_count = 0;
	unsigned char hun_count = 0;

	LCD_Cursor(cur_count);
	while( temp >= 100 ) {
		hun_count++;
		temp -= 100;
	}	
	
	LCD_WriteData(hun_count + '0');
	cur_count++;

	LCD_Cursor(cur_count);
	while( temp >= 10 ) {
		ten_count++;
		temp -= 10;
	}
	
	LCD_WriteData(ten_count + '0');
	cur_count++;
	
	LCD_Cursor(cur_count);
	LCD_WriteData('.');
	cur_count++;	
	
	LCD_Cursor(cur_count);
	LCD_WriteData((unsigned char) temp + '0');
	
	LCD_Cursor(0);
}

int main(void) {
	
	//Initializing LCD Display to output data from DHT sensor
	
	DDRC = 0xFF; PORTC = 0x00;   // set PORTC to output
	DDRD = 0xFF; PORTD = 0x00;   // set PORTD to output

	unsigned char* template = "Temp:   . C     Rh:   . %";
	unsigned char* error = "Error";
	
	LCD_init();
	LCD_ClearScreen ();
	LCD_DisplayString (1, template);

	_delay_ms(1000);
		
    while (1) {
		//call DHT sensor function defined in DHT.c
		if (dht_GetTempUtil(&temperature_int, &humidity_int) != -1) {
			
			LCD_print(temperature_int, 7);
			LCD_print(humidity_int, 21);
		}
		
		else {LCD_DisplayString (1, error);}
		
		_delay_ms(1500);
    }
}