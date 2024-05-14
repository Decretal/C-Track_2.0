/*
 * serial.c
 *
 * Created: 03.03.2024 14:45:52
 *  Author: Simon
 */ 


#include <avr/io.h>

/*
	SPI
*/

void spi_init()
{
	// Set MOSI (PB5), SCK (PB7), CSL (PA6) and CSSD (PB4) as outputs
	DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);
	DDRA |= (1 << PA6);
	
	PORTA |= (1 << PA6);
	
	// Enable SPI, Set as Master, Set clock rate fck/4
	SPCR |= (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0);
}

void spi_tx(uint8_t dat)
{
	SPDR = dat;
	while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_rx()
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

uint8_t spi_x(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}


/*
	UART
*/

void uart_init()
{
	int ubrr = 12;
	// Set baud rate
	UBRR1H = (ubrr>>8);
	UBRR1L = ubrr;

	// Enable receiver and transmitter
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);

	// Enable receive complete interrupt
	//UCSR1B |= (1 << RXCIE1);

	// Set frame format: 8 data, 1 stop bit
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
}


char uart_receive()
{
	// Wait for data to be received
	while (!(UCSR1A & (1 << RXC1)));
	// Return received data
	return UDR1;
}

void uart_transmit(char data)
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

void uart_sendStr(const char *data)
{
	while (*data != '\0')
	{
		uart_transmit(*data);
		data++;
	}
}