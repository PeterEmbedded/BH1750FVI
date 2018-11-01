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

/* 
 *  Digital Light sensor BH1750FVI (GY-302)
 */
  
#ifndef BH1750FVI_h
#define BH1750FVI_h

#include "Arduino.h"
#include "Wire.h"

/*
 * BH1750FVI driver class
 * Provides an easy interface to the BH1750FVI digital lightsensor
 */
class BH1750FVI {    

  public:

    typedef enum eDeviceAddress {
      k_DevAddress_L = 0x23,
      k_DevAddress_H = 0x5C
    } eDeviceAddress_t;

    typedef enum eDeviceMode {
      k_DevModeContHighRes     = 0x10,
      k_DevModeContHighRes2    = 0x11,
      k_DevModeContLowRes      = 0x13,
      k_DevModeOneTimeHighRes  = 0x20,
      k_DevModeOneTimeHighRes2 = 0x21,
      k_DevModeOneTimeLowRes   = 0x23
    } eDeviceMode_t;

    typedef enum eDeviceState {
      k_DevStatePowerDown = 0x00,
      k_DevStatePowerUp   = 0x01,
      k_DevStateReset     = 0x07
    } eDeviceState_t;
    
    /*
    * Constructor
    * Omits the need for an extra addresspin and sets the address to Low
    * @param DeviceMode, the mode to set the device in
    */
    BH1750FVI(eDeviceMode_t DeviceMode);
    
    /*
    * Constructor
    * @param AddressPin, pin number the device is attached to
    * @param DeviceAddress, I2C address of the device (High or Low)
    * @param DeviceMode, the mode to set the device in
    */
    BH1750FVI(uint8_t AddressPin, eDeviceAddress_t DeviceAddress, eDeviceMode_t DeviceMode);
    
    /*
     * The defacto standard begin call to initialize and start the driver
     * This will take care of setting the address, mode and powering up the device
     */
    void begin(void);

    /*
     * Get the measured light intensity from the device
     * @return The light intensity measured by the device in 'lux'
     */
    uint16_t GetLightIntensity(void);
    
    /*
     * Set the mode
     * @param DeviceMode, the mode to set the device in
     */
    void SetMode(eDeviceMode_t DeviceMode);

    /*
     * Activate sleep mode
     */
    void Sleep(void); 
    
    /*
     * Reset the device
     */
    void Reset(void);

       
  private:
    
    /*
     * Set the High or Low I2C address
     * @param DeviceAddress, the high or low address
     */
    void SetAddress(eDeviceAddress_t DeviceAddress);

    /*
     * Write data to the I2C port
     * @param Data, data to write
     */
    void I2CWrite(uint8_t Data);  

    uint8_t m_AddressPin;               //!< Addresspin number the device is connected to
    eDeviceAddress_t m_DeviceAddress;   //!< I2C address of the device
    eDeviceMode_t m_DeviceMode;         //!< Mode of the device
    bool m_AddressPinUsed;              //!< Constructed with or without the address pin
};

#endif
