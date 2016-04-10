// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HCPA-5V-U3
// This code is designed to work with the HCPA-5V-U3_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=HCPA-5V-U3_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// HCPA-5V-U3 I2C address is 0x28(40)
#define Addr 0x28

float cTemp = 0.0, fTemp = 0.0, humidity = 0.0;

void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "HCPA-5V-U3");
  Particle.variable("humidity", humidity);
  Particle.variable("cTemp", cTemp);

  // Initialise I2C communication as Master
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send start command
  Wire.write(0x80);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[2];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 4 byte of data
  Wire.requestFrom(Addr, 4);

  // Read 4 bytes of data
  // humidity msb, humidity lsb, cTemp msb, cTemp lsb
  if (Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }

  // Convert the data to 14-bits
  humidity = (((data[0] & 0x3F) * 256) + data[1]) / 16384.0 * 100.0;
  cTemp = (((data[2] * 256) + (data[3] & 0xFC)) / 4) / 16384.0 * 165.0 - 40.0;
  fTemp = (cTemp * 1.8) + 32;

  // Output data to dashboard
  Particle.publish("Relative humidity : ", String(humidity));
  Particle.publish("Temperature in Celsius : ", String(cTemp));
  Particle.publish("Temperature in Farhenheit : ", String(fTemp));
  delay(500);
}