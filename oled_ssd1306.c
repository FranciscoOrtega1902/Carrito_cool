

#include "prj.h"
#include "font6x8.h"
#include "font8x16.h"
#include "bateria.h"

// ----------------------------------------------------------------------------
 
const uint8_t ssd1306_init_sequence [] PROGMEM = {	// Initialization Sequence
	0xAE,			// Display OFF (sleep mode)
	0x20, 0b00,		// Set Memory Addressing Mode
					// 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
					// 10=Page Addressing Mode (RESET); 11=Invalid
	0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
	0xC8,			// Set COM Output Scan Direction
	0x00,			// ---set low column address
	0x10,			// ---set high column address
	0x40,			// --set start line address
	0x81, 0x3F,		// Set contrast control register
	0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
	0xA6,			// Set display mode. A6=Normal; A7=Inverse
	0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
	0xA4,			// Output RAM to Display
					// 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0xD5,			// --set display clock divide ratio/oscillator frequency
	0xF0,			// --set divide ratio
	0xD9, 0x22,		// Set pre-charge period
	0xDA, 0x12,		// Set com pins hardware configuration		
	0xDB,			// --set vcomh
	0x20,			// 0x20,0.77xVcc
	0x8D, 0x14,		// Set DC-DC enable
	0xAF			// Display ON in normal mode
	
};

 
// ----------------------------------------------------------------------------

void ssd1306_init(void)
{
    for (uint8_t i = 0; i < sizeof (ssd1306_init_sequence); i++)
    {
		ssd1306_send_command(pgm_read_byte(&ssd1306_init_sequence[i]));
	}
}

void ssd1306_send_command(uint8_t command)
{
	i2c_start();
	i2c_writebyte(SSD1306_SA);  // Slave address, SA0=0
	i2c_writebyte(0x00);	// write command
	i2c_writebyte(command);
	i2c_stop();
}

void ssd1306_setpos(uint8_t x, uint8_t y)
{
	i2c_start();
	i2c_writebyte(SSD1306_SA);  // Slave address, SA0=0
	i2c_writebyte(0x00);	// write command
	i2c_writebyte(0xb0 + y);
	i2c_writebyte(((x & 0xf0) >> 4) | 0x10); // | 0x10

    i2c_writebyte((x & 0x0f)); // | 0x01
	i2c_stop();
}

void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) 
{
	ssd1306_setpos(0, 0);
		i2c_start();
	i2c_writebyte(SSD1306_SA);
	i2c_writebyte(0x40);	//write data
	for (uint16_t i = 0; i < 128 * 8 / 4; i++) 
    {
		i2c_writebyte(p1);
		i2c_writebyte(p2);
		i2c_writebyte(p3);
		i2c_writebyte(p4);
	}
	i2c_stop();
}

void ssd1306_fill(uint8_t p) 
{
	ssd1306_fill4(p, p, p, p);
}

// ----------------------------------------------------------------------------

void ssd1306_char_font6x8(char ch) 
{
	uint8_t c = ch - 32;
	i2c_start();
	i2c_writebyte(SSD1306_SA);
	i2c_writebyte(0x40);	//write data
	for (uint8_t i = 0; i < 6; i++)
	{
		i2c_writebyte(pgm_read_byte(&ssd1306xled_font6x8[c * 6 + i]));
	}
	i2c_stop();
}

void ssd1306_bateria(int gh)
{
	//uint8_t c = ch - 32;
	i2c_start();
	i2c_writebyte(SSD1306_SA);
	i2c_writebyte(0x40);	//write data
	for (uint8_t i = 0; i < 16; i++)
	{
		i2c_writebyte(pgm_read_byte(&ssd1306xbateria[gh * 16 + i]));
	}
	i2c_stop();
}

void ssd1306_string_font6x8(char *s) 
{
	while (*s) 
    {
		ssd1306_char_font6x8(*s++);
	}
}

 
// ----------------------------------------------------------------------------

void ssd1306_string_font8x16xy(uint8_t x, uint8_t y, const char s[]) 
{
	uint8_t ch, j = 0;
	while (s[j] != '\0') 
    {
		ch = s[j] - 32;
		if (x > 120) 
        {
			x = 0;
			y++;
		}
		ssd1306_setpos(x, y);
		i2c_start();
	    i2c_writebyte(SSD1306_SA);
	    i2c_writebyte(0x40);	//write data
		for (uint8_t i = 0; i < 8; i++) 
        {
			i2c_writebyte(pgm_read_byte(&ssd1306xled_font8x16[ch * 16 + i]));
		}
		i2c_stop();
		ssd1306_setpos(x, y + 1);
		i2c_start();
    	i2c_writebyte(SSD1306_SA);
    	i2c_writebyte(0x40);	//write data
		for (uint8_t i = 0; i < 8; i++) 
        {
			i2c_writebyte(pgm_read_byte(&ssd1306xled_font8x16[ch * 16 + i + 8]));
		}
		i2c_stop();
		x += 8;
		j++;
	}
}

// ----------------------------------------------------------------------------

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[])
{
	uint16_t j = 0;
	uint8_t y;
	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		ssd1306_setpos(x0,y);
		i2c_start();
	i2c_writebyte(SSD1306_SA);
	i2c_writebyte(0x40);	//write data
		for (uint8_t x = x0; x < x1; x++)
		{
			i2c_writebyte(pgm_read_byte(&bitmap[j++]));
		}
		i2c_stop();
	}
}
