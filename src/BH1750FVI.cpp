/*
  MIT License
  
  Copyright (c) 2017 PeterEmbedded
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "BH1750FVI.h"
#include "Arduino.h"

BH1750FVI::BH1750FVI(eDeviceMode_t DeviceMode) :
  m_DeviceMode(DeviceMode)
{
  m_AddressPinUsed = false;
  m_DeviceAddress = k_DevAddress_L;
}

BH1750FVI::BH1750FVI(uint8_t AddressPin, eDeviceAddress_t DeviceAddress, eDeviceMode_t DeviceMode) :
  m_AddressPin(AddressPin),
  m_DeviceAddress(DeviceAddress),
  m_DeviceMode(DeviceMode)
{
  m_AddressPinUsed = true;
}

void BH1750FVI::begin(void)
{
  Wire.begin(); 
  I2CWrite(k_DevStatePowerUp);      // Turn it On 
  if (m_AddressPinUsed) {
    pinMode(m_AddressPin, OUTPUT);    // Set the correct pinmode
    digitalWrite(m_AddressPin, HIGH); // Address to high
    SetAddress(m_DeviceAddress);      // Set the address
  }
  SetMode(m_DeviceMode);            // Set the mode
}
  
void BH1750FVI::Sleep(void)
{
  I2CWrite(k_DevStatePowerDown); // Turn it off
}

void BH1750FVI::Reset(void)
{
  I2CWrite(k_DevStatePowerUp);  // Turn it on before we can reset it
  I2CWrite(k_DevStateReset );   // Reset
}

void BH1750FVI::SetAddress(eDeviceAddress_t DeviceAddress)
{
  if (m_AddressPinUsed) {
    m_DeviceAddress = DeviceAddress;
    switch (m_DeviceAddress) {
      case k_DevAddress_L:
        digitalWrite(m_AddressPin, LOW);
        break;
      case k_DevAddress_H:
        digitalWrite(m_AddressPin, HIGH);
        break;
      default:
        digitalWrite(m_AddressPin, HIGH);
        break;
    }
  }
}

void BH1750FVI::SetMode(eDeviceMode_t DeviceMode)
{
  m_DeviceMode = DeviceMode;
  delay(10);
  I2CWrite(m_DeviceMode);
}

uint16_t BH1750FVI::GetLightIntensity(void)
{
  uint16_t Value = 0;

  Wire.requestFrom(m_DeviceAddress, 2);
  Value = Wire.read();
  Value <<= 8;
  Value |= Wire.read();

  return Value / 1.2;
}

void BH1750FVI::I2CWrite(uint8_t Data)
{
  Wire.beginTransmission(m_DeviceAddress);
  Wire.write(Data);
  Wire.endTransmission();
}
