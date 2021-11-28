
#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

  
#define SSD1306_SA		0x78	// Slave address
 

// ----------------------------------------------------------------------------

#define ssd1306_clear()   ssd1306_fill(0);
#define ssd1306_char(c)   ssd1306_char_font6x8(c)
#define ssd1306_string(s) ssd1306_string_font6x8(s)


// ----------------------------------------------------------------------------

void ssd1306_init(void);
void ssd1306_send_command(uint8_t command);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_fill4(uint8_t, uint8_t, uint8_t, uint8_t);
void ssd1306_fill2(uint8_t p1, uint8_t p2);
void ssd1306_fill(uint8_t p);
void ssd1306_fillscreen(uint8_t fill);
void ssd1306_char_font6x8(char ch);
void ssd1306_string_font6x8(char *s);
void ssd1306_numdec_font6x8(uint16_t num);
void ssd1306_numdecp_font6x8(uint16_t num);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);

// ----------------------------------------------------------------------------
void ssd1306_string_font8x16xy(uint8_t x, uint8_t y, const char ch[]);

#endif
