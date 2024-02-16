#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace veml6030 {

    //Register IDs
    enum class VEML6030_16BIT_REGISTERS : uint8_t {
        ALS_CONF = 0,//W: ALS gain, integration time, interrupt, and shut down
        ALS_WH = 1,//W: ALS high threshold window setting 
        ALS_WL = 2,//W: ALS low threshold window setting
        Power_Saving = 3,//W: Set (15 : 3) 0000 0000 0000 0b
        ALS = 4, //R: MSB, LSB data of whole 16 bits
        WHITE = 5, //R: MSB, LSB data of whole 16 bits
        ALS_INT = 6, //R: ALS INT trigger event
    };

    //I2C device address
    static const uint8_t defADDR = 0x48;
    static const uint8_t altADDR = 0x10;

    //
    //ALS_CONF REGISTER
    //
    //list all integration times BITS 9:6
    enum class veml6030IntegrationTime :uint8_t {
        veml6030_INTEGRATION_25MS = 1100,
        veml6030_INTEGRATION_50MS = 1000,
        veml6030_INTEGRATION_100MS = 0000,
        veml6030_INTEGRATION_200MS = 0001,
        veml6030_INTEGRATION_400MS = 0010,
        veml6030_INTEGRATION_800MS = 0011,
    };
    //gain settings BITS 12:11
    enum  class veml6030Gain : uint8_t {
        veml6030_GAIN_1x = 00,
        veml6030_GAIN_2x = 01,
        veml6030_GAIN_1_8x = 10,
        veml6030_GAIN_1_4x = 11,
    };
    //Power saving modes set register 0x03 bits 2-1 as follows. Enable power saving mode by setting bit 0 to 1.
    enum class PowerSavingMode : uint8_t {
        PSM1 = 00,
        PSM2 = 01,
        PSM3 = 10,
        PSM4 = 11,
        PSMenabled = 1,
    }; 

class veml6030Component : public PollingComponent, public i2c::I2CDevice {
    public:

    //ESPHOME framework functions
    float get_setup_priority() const override {return get_setup_priority::DATA;}
    void setup() override;
    void dump_config() override;
    void update() override;
    void loop() override;

    //Configuration setters

// REG0x00, bits [12:11]
    // This function sets the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    void setGain(float gainVal);

    // REG0x00, bits [12:11]
    // This function reads the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    float readGain();

    // REG0x00, bits[9:6]
    // This function sets the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower sensor refresh times. 
    void setIntegTime(uint16_t time);

    // REG0x00, bits[9:6]
    // This function reads the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower sensor refresh times. 
    uint16_t readIntegTime();

    // REG0x00, bits[5:4]
    // This function sets the persistence protect number i.e. the number of
    // values needing to crosh the interrupt thresholds.
    void setProtect(uint8_t protVal);

    // REG0x00, bits[5:4]
    // This function reads the persistence protect number i.e. the number of 
    // values needing to crosh the interrupt thresholds.
    uint8_t readProtect();

    // REG0x00, bit[1]
    // This function enables the Ambient Light Sensor's interrupt. 
    void enableInt();

    // REG0x00, bit[1]
    // This function disables the Ambient Light Sensor's interrupt. 
    void disableInt();

    // REG0x00, bit[1]
    // This function checks if the interrupt is enabled or disabled. 
    uint8_t readIntSetting();

    // REG0x00, bit[0]
    // This function powers down the Ambient Light Sensor. The light sensor will
    // hold onto the last light reading which can be acessed while the sensor is 
    // shut down. 0.5 micro Amps are consumed while shutdown. 
    void shutDown();

    // REG0x00, bit[0]
    // This function powers up the Ambient Light Sensor. The last value that was
    // read during shut down will be overwritten on the sensor's subsequent read.
    // After power up, a small 4ms delay is applied to give time for the internal
    // osciallator and signal processor to power up.   
    void powerOn();

    // REG0x03, bit[0]
    // This function enables the current power save mode value and puts the Ambient
    // Light Sensor into power save mode. 
    void enablePowSave();

    // REG0x03, bit[0]
    // This function disables the current power save mode value and pulls the Ambient
    // Light Sensor out of power save mode. 
    void disablePowSave();

    // REG0x03, bit[0]
    // This function checks to see if power save mode is enabled or disabled. 
    uint8_t readPowSavEnabled();

    // REG0x03, bit[2:1]
    // This function sets the power save mode value. It takes a value of 1-4. Each
    // incrementally higher value descreases the sampling rate of the sensor and so
    // increases power saving. The datasheet suggests enabling these modes when
    // continually sampling the sensor. 
    void setPowSavMode(uint16_t modeVal);

    // REG0x03, bit[2:1]
    // This function reads the power save mode value. The function above takes a value of 1-4. Each
    // incrementally higher value descreases the sampling rate of the sensor and so
    // increases power saving. The datasheet suggests enabling these modes when
    // continually sampling the sensor. 
    uint8_t readPowSavMode();

    // REG0x06, bits[15:14]
    // This function reads the interrupt register to see if an interrupt has been
    // triggered. There are two possible interrupts: a lower limit and upper limit 
    // threshold, both set by the user.  
    uint8_t readInterrupt();

    // REG0x02, bits[15:0]
    // This function sets the lower limit for the Ambient Light Sensor's interrupt. 
    // It takes a lux value as its paramater.
    void setIntLowThresh(uint32_t luxVal);
    
    // REG0x02, bits[15:0]
    // This function reads the lower limit for the Ambient Light Sensor's interrupt. 
    uint32_t readLowThresh();

    // REG0x01, bits[15:0]
    // This function sets the upper limit for the Ambient Light Sensor's interrupt. 
    // It takes a lux value as its paramater.
    void setIntHighThresh(uint32_t luxVal);

    // REG0x01, bits[15:0]
    // This function reads the upper limit for the Ambient Light Sensor's interrupt. 
    uint32_t readHighThresh();

    // REG[0x04], bits[15:0]
    // This function gets the sensor's ambient light's lux value. The lux value is
    // determined based on current gain and integration time settings. If the lux
    // value exceeds 1000 then a compensation formula is applied to it. 
    uint32_t readLight();

    // REG[0x05], bits[15:0]
    // This function gets the sensor's ambient light's lux value. The lux value is
    // determined based on current gain and integration time settings. If the lux
    // value exceeds 1000 then a compensation formula is applied to it. 
    uint32_t readWhiteLight();

  private:

    uint8_t _address;
    
    // This function compensates for lux values over 1000 per datasheet
    uint32_t _luxCompensation(uint32_t _luxVal);

    // The lux value of the Ambient Light sensor depends on both the gain and the
    // integration time settings. This function determines which conversion value
    // to use by using the bit representation of the gain as an index to look up
    // the conversion value in the correct integration time array. It then converts 
    // the value and returns it.  
    uint32_t _calculateLux(uint16_t _lightBits);

    // This function does the opposite calculation then the function above. The interrupt
    // threshold values given by the user are dependent on the gain and
    // intergration time settings. As a result the lux value needs to be
    // calculated with the current settings and this function accomplishes
    // that.  
    uint16_t _calculateBits(uint32_t _luxVal);

    // This function writes to a 16 bit register. Paramaters include the register's address, a mask 
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void _writeRegister(uint8_t _wReg, uint16_t _mask, uint16_t _bits, uint8_t _startPosition);

    // This function reads a 16 bit register. It takes the register's
    // address as its' parameter.
    uint16_t _readRegister(uint8_t _reg);
    
}

} //namespace veml6030

} //namespace esphome