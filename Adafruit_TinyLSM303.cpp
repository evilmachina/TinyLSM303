/***************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
/***************************************************************************
  Copyright (C) 2014 (Marcus Olsson)
  Modified the code to be able run on Adafruit GEMMA and TRINKET (Attiny85)
  Removed code for magnentometer/compass
 ***************************************************************************/

#include <Adafruit_TinyLSM303.h>

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
bool Adafruit_TinyLSM303::begin()
{
  TinyWireM.begin();

  // Enable the accelerometer
  write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27);
  
  return true;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
void Adafruit_TinyLSM303::read()
{
  // Read the accelerometer
  TinyWireM.beginTransmission((byte)LSM303_ADDRESS_ACCEL);
  TinyWireM.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);

  // Wait around until enough data is available
  while (TinyWireM.available() < 6);

  uint8_t xlo = TinyWireM.read();
  uint8_t xhi = TinyWireM.read();
  uint8_t ylo = TinyWireM.read();
  uint8_t yhi = TinyWireM.read();
  uint8_t zlo = TinyWireM.read();
  uint8_t zhi = TinyWireM.read();

  // Shift values to create properly formed integer (low byte first)
  accelData.x = (xlo | (xhi << 8));
  accelData.y = (ylo | (yhi << 8));
  accelData.z = (zlo | (zhi << 8));
 
}

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
void Adafruit_TinyLSM303::write8(byte address, byte reg, byte value)
{
  TinyWireM.beginTransmission(address);
  TinyWireM.write(reg);
  TinyWireM.write(value);
  TinyWireM.endTransmission();
}

byte Adafruit_TinyLSM303::read8(byte address, byte reg)
{
  byte value;

  TinyWireM.beginTransmission(address);
  TinyWireM.write(reg);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(address, (byte)1);
  value = TinyWireM.read();
  TinyWireM.endTransmission();

  return value;
}
