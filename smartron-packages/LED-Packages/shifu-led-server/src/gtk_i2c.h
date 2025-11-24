#ifndef _GTK_I2C_H
#define _GTK_I2C_H

int gtk_i2c_read(int bus_id, unsigned char slvAddr, unsigned char op_code, unsigned char *data, int len);
int gtk_i2c_write(int bus_id, unsigned char slvAddr, unsigned char op_code, unsigned char *data, int len);
int gtk_i2c_read_wo_opcode(int bus_id, unsigned char slvAddr, unsigned char *data, int len);

#endif /* _GTK_I2C_H */
