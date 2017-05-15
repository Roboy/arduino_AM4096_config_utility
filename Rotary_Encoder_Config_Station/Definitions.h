#include "Rotary_Encoder_Settings.h"

bool readMemory(uint8_t deviceaddress, uint8_t eeaddress, byte* rdata);
bool writeMemory(uint8_t deviceaddress, uint8_t eeaddress, byte* wdata);



struct encoderSettings {
  bool interpolator_power;
  bool AGC_disabled; // true = disabled!
  bool regulator_35V;
  uint8_t I2C_address;
  bool incremental_ABRi_disabled;
  bool output_sinusoidal; // false = UVW, Tacho direction; true = sinusoidal
  bool error_amplitude; // true = ouput amplitude level signal instead of error on PIN
  bool sign_invert; // Inverts the rotation direction
  uint16_t zero_pos;
  bool absolute_out; // false = relative out (on PIN)
  uint8_t histeresis_value; // 7 bit
  bool tacho_out; // false = output position data
  uint8_t linear_v_period; // 2bit; 0 = 360°; 1 = 180°; 2 = 90°; 3 = 45°
  uint8_t Sth;
  uint8_t UVW_period;
  uint8_t interpolator_factor;
};

struct encoderState {
  bool data_valid;
  bool magnet_too_close;
  bool magnet_too_far;
  uint16_t abs_pos; // 12 bit
  uint16_t rel_pos; // relative to zeroed data
  uint16_t tacho; // 10 bit
  bool tacho_overflow;
  uint8_t AGC_gain; // 0 - 16 
};

