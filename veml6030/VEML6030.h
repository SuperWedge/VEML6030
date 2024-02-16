#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace veml6030 {

    //Register IDs
    static const uint8_t ALS_CONF = 0x00;
    static const uint8_t ALS_WH = 0x01;
    static const uint8_t ALS_WL = 0x02;
    static const uint8_t Power_Saving = 0x03;
    static const uint8_t ALS = 0x04;
    static const uint8_t WHITE = 0x05;
    static const uint8_t ALS_INT = 0x06;
    static const uint8_t ID = 0x07;

    // Other important constants
    static const uint8_t DEVICE_ID = 0x48;
    static const uint8_t 

    //PSM
    static const uint8_t PSM1 0x00;
    static const uint8_t PSM2 0x01;
    static const uint8_t PSM3 0x10;

class veml6030Component : public PollingComponent, public i2c::I2CDevice {
    public:
    void setup() override;
    void dump_config() override;
    void update() override {this->publish_state(this->read_lx_());}
    float get_setup_priority() const override {return get_setup_priority::DATA;}
}

}

}