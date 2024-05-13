#define F_CPU 1000000
#define T_ID 1

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdio.h>

#include "serial.h"
#include "GPS.h"
#include "lora.h"
#include "SD/ff.h"


FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */



int main (void)
{
	FRESULT fr;
	char buff[82];
	Point p;
	int i;
	
	bool lora;
	
	DDRD |= (1 << PIND4); // ON_OFF Pin From GPS Module
	PORTD &= ~(1 << PD4); // set ON_OFF LOW


	// Initialize Serial communication
	uart_init();
	spi_init();
	_delay_ms(1000); // wait till GPS and LoRa are ready
	
	
	/*
		The GPS Module is default in hibernate mode
		wake up with pulse on PD4
	*/
	PORTD |= (1 << PD4);
	_delay_ms(100);
	PORTD &= ~(1 << PD4);
	
	/*
		SD-Cards in default mode are not a SPI-Device
		To avoid conflicts the SD-Card has to be the first device initialized
	*/
	f_mount(&FatFs, "", 0);
	
	
	lora = lora_init(); // LoRa init
	
	lora_setSpreadingFactor(7);
	//lora_setCodingRate4(8);
	lora_set_bandwidth(0b0111); // 125 kHz
	lora_putd("01 06 53 40 11 04 24 4782124 1349760", 36);
	
	
	
	// Init GPS Module
	// disable all except RMC
	
	uart_sendStr("$PSRF103,0,0,0,1*24\r\n");
	uart_sendStr("$PSRF103,1,0,0,1*25\r\n");
	uart_sendStr("$PSRF103,2,0,0,1*26\r\n");
	uart_sendStr("$PSRF103,3,0,0,1*27\r\n");
	uart_sendStr("$PSRF103,5,0,0,1*21\r\n");
	uart_sendStr("$PSRF103,6,0,0,1*22\r\n");
	uart_sendStr("$PSRF103,7,0,0,1*23\r\n");
	uart_sendStr("$PSRF103,8,0,0,1*2C\r\n");
	uart_sendStr("$PSRF103,9,0,0,1*2D\r\n");
		
	uart_sendStr("$PSRF103,4,0,20,1*12\r\n");
	


	fr = f_open(&Fil, "pos.txt", FA_WRITE | FA_CREATE_ALWAYS);	// Create a file
	
	if (fr == FR_OK)
	{
		f_printf(&Fil, "YY/MM/DD HH:MM:SS;LL.LLLLL;BB.BBBBB\r\n");
		fr = f_close(&Fil);
	}
	
	
	
	i = -1;
	
	int j = 0;
	
	while (1)
	{
		while ((buff[++i] = uart_receive()) != '\n');
		buff[i] = '\0';
		
		//j++;
		
		//if (j != 5) continue;
		//j = 0;
		
		if (parseRMC(buff, &p))
		{
			fr = f_open(&Fil, "pos.txt", FA_OPEN_APPEND | FA_WRITE);
			
			if (fr == FR_OK)
			{
				f_printf(&Fil, "%02d/%02d/%02d %02d:%02d:%02d;%f;%f\r\n", p.day, p.month, p.year,  p.hour, p.minutes, p.seconds, p.lat, p.lng);
				fr = f_close(&Fil);
			}
			
			#ifdef T_ID
			
			if (lora)
			{
				int len = sprintf(buff, "%02hu %02hu %02hu %02hu %02hu %02hu %02hu %" PRId32 " %" PRId32 "",
					T_ID,
					p.hour,
					p.minutes,
					p.seconds,
					p.day,
					p.month,
					p.year,
					(int32_t)(100000* p.lat),
					(int32_t)(100000* p.lng));
	
				lora_putd(buff, len);
			}
			
			#endif // T_ID
		}
		
		i = -1;
	}
	
	return 0;
}



