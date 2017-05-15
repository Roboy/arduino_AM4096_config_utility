

/**
 * Original settings were (0x):
 * 0: 80 80
 * 1: 80 00
 * 2: 00 00
 * 3: 08 00
 * 
 * Our settings are:
 * 0: 11 00
 * 1: 80 00
 * 2: 00 00
 * 3: 00 00
 * 
 */


/**
 * Settings that apply to every chip and should be checked
 * vs. individual settings
 */
byte default_settings[][2] = {
  {0b00010001, 0b00000000}, // (interpolator, agc; no pdie, 5V; ADDR)
  {0b10000000, 0b00000000}, // (no abri, uvw, error, SIGN; ZERO)
  {0b00000000, 0b00000000}, // (0;0; HYSTERESIS)
  {0b10000000, 0b00000000}  // (vout, 360°, ssi00, TACHO RANGE, uvw1, INTERPOL 4096)
};

/**
 * 1 marks the bits that belong to the default settings
 * (to check the default settings)
 */
byte default_settings_map[][2] = {
  {0b11111111, 0b10000000}, // (ADDR)
  {0b11100000, 0b00000000}, // (SIGN; ZERO)
  {0b11111111, 0b10000000}, // (HYSTERESIS)
  {0b11111111, 0b11111111}  // ()
};

