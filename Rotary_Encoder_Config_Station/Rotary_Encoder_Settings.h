

/**
 * Original settings were (0x):
 * 0: 80 80
 * 1: 80 00
 * 2: 00 00
 * 3: 08 00
 * 
 * Our settings are:
 * 0: 11 00  (interpolator, agc; no pdie, 5V; ADDR)
 * 1: 80 00  (no abri, uvw, error, SIGN; ZERO)
 * 2: 00 00  (0;0; relative out, HYSTERESIS)
 * 3: 00 00  (vout, 360°, ssi00, TACHO RANGE, uvw1, INTERPOL 4096)
 * 
 */


// "Reg35"-Value
const byte default_settings_5v = 0; // 3v
// Accuracy vs Measuring Range
const byte default_settings_tacho_range = 0b101; // max 64rps (3840rpm)
// "Res"-Value
const byte default_settings_interpolation = 0; // 4096 steps / max 500Hz
// "Hist"-Value
const byte default_settings_hysteresis = 0; // no digital hysteresis (only analog)

/**
 * Settings that apply to every chip and should be checked
 * vs. individual settings
 */
const byte default_settings[][2] = {
  {
    0b00010000 | default_settings_5v & 0b1,
    0b00000000},
  {
    0b10000000,
    0b00000000},
  {
    0b00000000,
    0b00000000 | (default_settings_hysteresis&0b1111111)},
  {
    0b00000000 | (default_settings_tacho_range>>2)&0b1,
    0b00000000 | ((default_settings_tacho_range&0b11)<<6) | (default_settings_interpolation&0b111)}
};

/**
 * 1 marks the bits that belong to the default settings
 * (to check the default settings)
 */
const byte default_settings_mask[][2] = {
  {0b11111111, 0b10000000}, // (ADDR)
  {0b11100000, 0b00000000}, // (SIGN; ZERO)
  {0b11111111, 0b11111111}, // ()
  {0b11111111, 0b11111111}  // ()
};

