/*
 * lora.c
 *
 * Created: 06.03.2024 20:54:45
 *  Author: Simon
 */
#include <avr/io.h>
#include "lora.h"
#include "serial.h"

#include <stdbool.h>

bool lora_init()
{
	// SPI is already done in spi_init
	uint8_t v = lora_readRegister(REG_VERSION);
	if (v != 0x12) return false;
	
	lora_sleep();
	
	lora_setFrequency(868.3e6);
	
	lora_writeRegister(REG_FIFO_TX_BASE_ADDR, 0);
	lora_writeRegister(REG_FIFO_RX_BASE_ADDR, 0);
	
	
	// set LNA boost
	lora_writeRegister(REG_LNA, lora_readRegister(REG_LNA) | 0x03);
	// set auto AGC
	lora_writeRegister(REG_MODEM_CONFIG_3, 0x04);
	
	// set output power
	// max 12.75 dBm
	lora_setTxPwr(12);
	
	lora_idle();
	
	return true;
}

void lora_set_bandwidth( uint8_t mode ) {
	//Datasheet page 112

	lora_writeRegister( REG_MODEM_CONFIG_1, ( lora_readRegister(REG_MODEM_CONFIG_1) & 0b00001111 ) | (mode << 4) );
}

void lora_setSpreadingFactor(int sf)
{
	if (sf < 6)
	{
		sf = 6;
	}
	else if (sf > 12)
	{
		sf = 12;
	}
	
	if (sf == 6)
	{
		lora_writeRegister(REG_DETECTION_OPTIMIZE, 0xc5);
		lora_writeRegister(REG_DETECTION_THRESHOLD, 0x0c);
	}
	else
	{
		lora_writeRegister(REG_DETECTION_OPTIMIZE, 0xc3);
		lora_writeRegister(REG_DETECTION_THRESHOLD, 0x0a);
	}
	
	lora_writeRegister(REG_MODEM_CONFIG_2, (lora_readRegister(REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
}

void lora_putd(char* buf, uint8_t len )
{
	if( len == 0 ) return;
	
	//Datasheet page 38
	
	//The LoRaTM FIFO can only be filled in Standby mode.
	//1. Mode request STAND-BY
	lora_idle();
	
	//2. TX inits
	lora_writeRegister(REG_MODEM_CONFIG_1, lora_readRegister(REG_MODEM_CONFIG_1) & 0xfe);

	//3. Write data to FIFO
	lora_writeRegister( REG_FIFO_ADDR_PTR, 0 );
	
	for( uint8_t i = 0; i < len; i++ ) lora_writeRegister( REG_FIFO, buf[i] );
	lora_writeRegister( REG_PAYLOAD_LENGTH, len );

	//4. Mode request TX	
	lora_writeRegister( REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX );

	//5. Wait for IRQ TxDone
	uint8_t irq;
	while( (irq = lora_readRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0 );
	lora_writeRegister( REG_IRQ_FLAGS, irq );
	
	//6. Auto mode change to STAND-BY
}


void lora_setFrequency(long frequency)
{
	uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

	lora_writeRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
	lora_writeRegister(REG_FRF_MID, (uint8_t)(frf >> 8));
	lora_writeRegister(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

// Over Current Protection
void lora_setOCP( uint8_t mA)
{
	uint8_t ocpTrim = 27;

	if (mA <= 120) {
		ocpTrim = (mA - 45) / 5;
		} else if (mA <=240) {
		ocpTrim = (mA + 30) / 10;
	}

	lora_writeRegister(REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void lora_setCodingRate4(int denominator)
{
	if (denominator < 5) {
		denominator = 5;
		} else if (denominator > 8) {
		denominator = 8;
	}

	int cr = denominator - 4;

	lora_writeRegister(REG_MODEM_CONFIG_1, (lora_readRegister(REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

/*
void lora_setTxPwr(int level)
{
	if (level > 17) {
		if (level > 20) {
			level = 20;
		}

		// subtract 3 from level, so 18 - 20 maps to 15 - 17
		level -= 3;

		// High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
		lora_writeRegister(REG_PA_DAC, 0x87);
		lora_setOCP(140);
		} else {
		if (level < 2) {
			level = 2;
		}
		//Default value PA_HF/LF or +17dBm
		lora_writeRegister(REG_PA_DAC, 0x84);
		lora_setOCP(100);
	}

	lora_writeRegister(REG_PA_CONFIG, PA_BOOST | (level - 2));
}*/

void lora_setTxPwr(int level)
{
	// PA BOOST
	if (level > 17) {
		if (level > 20) {
			level = 20;
		}

		// subtract 3 from level, so 18 - 20 maps to 15 - 17
		level -= 3;

		// High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
		lora_writeRegister(REG_PA_DAC, 0x87);
		lora_setOCP(140);
		} else {
		if (level < 2) {
			level = 2;
		}
		//Default value PA_HF/LF or +17dBm
		lora_writeRegister(REG_PA_DAC, 0x84);
		lora_setOCP(100);
	}

	lora_writeRegister(REG_PA_CONFIG, PA_BOOST | (level - 2));
}


void lora_sleep()
{
	lora_writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void lora_idle()
{
	lora_writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}



void lora_writeRegister(uint8_t addr, uint8_t val)
{
	lora_exchange(addr | 0x80, val);
}

uint8_t lora_exchange(uint8_t addr, uint8_t val)
{
	register uint8_t data;

	//Set chip select (slave select or NSS) pin to low to indicate spi data transmission
	PORTA &= ~(1 << PA6);
	//Send register address
	spi_tx(addr);
	//Read or send register value from module
	data = spi_x(val);
	//End transmission
	PORTA |= (1 << PA6);

	return data;
}

uint8_t lora_readRegister(uint8_t reg)
{
	//To read register, 8th bit has to be set to 0, which is achieved with masking with 0x7f
	return lora_exchange( reg & 0x7f, 0x00 );
}