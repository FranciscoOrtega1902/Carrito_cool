
//---------------------------
// SOFT_I2C.c
//---------------------------

#include "prj.h"


#define SCL_low()              SCL_PORT  &= ~_BV(SCL_BIT)   
#define SCL_high()             SCL_PORT  |=  _BV(SCL_BIT)   

#define SDA_low()              SDA_PORT  &= ~_BV(SDA_BIT)   
#define SDA_high()             SDA_PORT  |=  _BV(SDA_BIT)   


void i2c_init()
{
	SDA_DIR |= (1 << SDA_BIT);	// Set port as output
	SCL_DIR |= (1 << SCL_BIT);	// Set port as output
}

void i2c_start(void)
{

    SCL_high();
    SDA_high();
    SDA_low();
    SCL_low();
}

void i2c_stop(void)
{

    SCL_low();
    SDA_low();
    SCL_high();
    SDA_high();
}

void i2c_writebyte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if ((byte << i) & 0x80)
        {
		
            SDA_high();
            }
		else
        {
		
            SDA_low();
            }
		
	
        SCL_high();
        SCL_low();
	}

    SDA_high();
    SCL_high();
    SCL_low();
}


