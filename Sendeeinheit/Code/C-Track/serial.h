/*
 * serial.h
 *
 * Created: 03.03.2024 14:46:09
 *  Author: Simon
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

void spi_init();
void spi_tx(uint8_t dat);
uint8_t spi_rx();
uint8_t spi_x(uint8_t data);


void uart_init();
char uart_receive();
void uart_transmit(char data);
void uart_sendStr(const char *data);


#endif /* SERIAL_H_ */