C335 Lab 8
Priya Saravanan
psaravan

Q1) Since the nunchuck communicates over I2C, as long as I@C is initialized, the nunchuck doesn't need the pins to be initialized and it will communicate

Q2) In f3d_i2c1_read, the function mainly deals with getting the device that needs to communicate and does the transfer of bytes for that device. The f3d_nunchuck_read function takes the data values captured by the nunchuck (like  wehn c and z buttons are pressed, accelerometer values etc.) and stores them into a list where bitwise operations are carried out on them. This means that the f3d_i2c_read functions helps the nunchuck communicate and the f3d_nunchuck_read function deals with the values that the nunchuck captures. 

Q3) typedef struct nunchuk_data {
  unsigned char jx; // joystick movement in x direction
  unsigned char jy; // joystick movement in y direction
  unsigned short ax; // nunchuck accelerometer in x direction
  unsigned short ay; // nunchuck accelerometer in y direction
  unsigned short az; // nunchuck accelerometer in z direction
  unsigned char c; // c button
  unsigned char z; // z button
} nunchuk_t;

Q4) The switch cases will refer to the c or z button value of the structure defined in f3d_nunchuck.h and set it to either 1 or 0 depending on the case. 

Difficulties: 