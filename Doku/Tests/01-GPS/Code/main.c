/*
 * GccApplication420.c
 *
 * Created: 12.12.2023 15:26:07
 * Author : koeni
 */ 
#define F_CPU (unsigned long)1e6
#define BAUD_RATE 4800

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "mmc_config.h"
#include "mmc.h"
#include "file.h"
#include "fat.h"

static void timer0_init(void);
volatile uint8_t TimingDelay;

ISR (TIMER0_COMPA_vect)
{
	TimingDelay = (TimingDelay==0) ? 0 : TimingDelay-1;
}

static void timer0_init(){

	TimingDelay = 0;
/*
	TCCR0A = 1<<WGM01;
	TIMSK0 = 1<<OCIE0A;

	TCNT0 = 0x9B;
	OCR0A = 0x64;

	TCCR0B = 0x04;*/
}

void uart1_init() {
	int ubrr = 12;
	// Set baud rate
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;

	// Enable receiver and transmitter
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);

	// Enable receive complete interrupt
	//UCSR1B |= (1 << RXCIE1);

	// Set frame format: 8 data, 1 stop bit
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
}

char uart1_receive() {
	// Wait for data to be received
	while (!(UCSR1A & (1 << RXC1)))
	;
	// Return received data
	return UDR1;
}

void uart_transmit(char data) {
	while (!(UCSR1A & (1 << UDRE1)))
	;
	
	UDR1 = data;
}

void usart_send_str(const char *data) {
	while (*data != '\0') {
		uart_transmit(*data);
		data++;
	}
}

uint8_t file_name [] = "file.txt";


int main(void) {
	unsigned char buff[512];
	volatile int i = -1;
	
	
	DDRD |= (1 << PIND4);
	
	PORTD |= (1 << PD4);
	_delay_ms(100);
	PORTD &= ~(1 << PD4);
	
	uart1_init();
	timer0_init();
	sei();
	
	while ( FALSE == mmc_init()) 
	{
		// try again
	}
	
	const char str[] = "$PSRF103,0,0,5,1*21\r\n";
	
	if( FALSE == fat_loadFatData()) return 1;	
	
	if(MMC_FILE_CREATED == ffopen(file_name,'c') ){
		ffwrites(str);
		ffclose();
	}
	
	
	usart_send_str(str);
	usart_send_str("$PSRF103,1,0,0,1*25\r\n");
	usart_send_str("$PSRF103,2,0,0,1*26\r\n");
	usart_send_str("$PSRF103,3,0,0,1*27\r\n");
	usart_send_str("$PSRF103,4,0,0,1*20\r\n");
	usart_send_str("$PSRF103,5,0,0,1*21\r\n");
	usart_send_str("$PSRF103,6,0,0,1*22\r\n");
	usart_send_str("$PSRF103,7,0,0,1*23\r\n");
	usart_send_str("$PSRF103,8,0,0,1*2C\r\n");
	usart_send_str("$PSRF103,9,0,0,1*2D\r\n");
	
	while (1)
	{
		while ((buff[++i] = uart1_receive()) != '\n');
		
		buff[i] = '\0';
		
		if (MMC_FILE_OPENED == ffopen(file_name, 'r'))
		{
			ffseek(file.length);
			ffwrites(buff);
			ffclose();
		}
		i = -1;
	}
	
	return 0;
}