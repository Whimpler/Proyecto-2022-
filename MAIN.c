/*
 * proyecto_riego.c
 *
 * Created: 9/8/2022 19:07:37
 * Author : Dell
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
char buf[512];

#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

#include "DEF_ATMEGA328P.h"
#include "UART.h"
#include "SWseriale.h"
#include "ADC.h"

int contador=0;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{

    float temperatura;
    float humedad;
    uint8_t contador=200;
	uint8_t modo;
	uint8_t bomba;
	DDRB = 0b00000011 ;
	PORTB= 0b00000000 ;
    
    
    sei();
    SWseriale_begin(); // Initialize INT1, Timer2, Pin 3 (Input, Pull-up) and Pin 4 (Output)
    _delay_ms(10);
    UART_init();
    _delay_ms(10);
    while (1) 
    {
		modo=SWseriale_read();
		bomba=SWseriale_read();
		contador ++;
		if(contador>=200){
			contador=0;
			temperatura=map(ADC_GetData(0),70,100,0,40);
			humedad=map(ADC_GetData(1),1024,0,0,99);
			_delay_ms(100);
			snprintf(buf,sizeof buf,"%s%s%s%s%s%s%s",flot2str(temperatura),",",flot2str(humedad),",");
			serial_print_str(buf);
			serial_println_str(flot2str(humedad));
			 SWseriale_write("x0.val=",7);
			 SWseriale_write(flot2str(temperatura),5);
			 SWseriale_write("0xff",4);
			 SWseriale_write("0xff",4);
			 SWseriale_write("0xff",4);
			 SWseriale_write("  ",2);
			 _delay_ms(1000);
			 SWseriale_write("x1.val=",7);
			 SWseriale_write(flot2str(humedad),5);
			 SWseriale_write("0xff",4);
			 SWseriale_write("0xff",4);
			 SWseriale_write("0xff",4);
			 SWseriale_write("  ",2);
			 _delay_ms(1000);
			 SWseriale_write("get x2.val",11);
			 SWseriale_write("0xff",4);
			 SWseriale_write("0xff",4);
			 SWseriale_write("0xff",4);
			 SWseriale_write("  ",2);
			 _delay_ms(1000);
			 
			 if (modo==1){
				if (humedad<60 && temperatura>34){
						
					PORTB=0b00000011;
					_delay_ms(10);
						
				}
				else {
					PORTB=0b00000000;
					_delay_ms(10);
				}
			}
			else{
					if(bomba==1){
							PORTB=0b00000011;
							_delay_ms(10);
									
					}
					else {
							PORTB=0b00000000;
							_delay_ms(10);
					}

				}	
			}
			
		}
}
