#ifndef SOFT_I2C_H
#define SOFT_I2C_H

void i2c_init();

void i2c_start(void);
void i2c_stop(void);
void i2c_writebyte(uint8_t byte);

#endif
