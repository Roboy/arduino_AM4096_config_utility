
bool readMemory(uint8_t deviceaddress, uint8_t eeaddress, byte* rdata)
{
  // Address 0 hangs if read "manually" but we can read it by reading without a request
  if(eeaddress > 0){
    Wire.beginTransmission(deviceaddress);
    Wire.write(eeaddress);
    if(Wire.endTransmission(false) != 0){ // Restart Line for request
      Wire.endTransmission(true);
      return false;
    }
  }
  
  Wire.requestFrom(deviceaddress, (uint8_t) 2, (uint8_t) true);
  rdata[0] = Wire.read();
  rdata[1] = Wire.read();
  return true;
}

bool writeMemory(uint8_t deviceaddress, uint8_t eeaddress, byte* wdata)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write(eeaddress); // LSB
  Wire.write(wdata, 2); // LSB
  if(Wire.endTransmission(true) != 0){
    return false;
  }
  return true;
}

bool writeMemoryCheck(uint8_t deviceaddress,uint8_t eeaddress, byte* wdata)
{
  if(!writeMemory(deviceaddress, eeaddress, wdata)){
    return false;
  }
  byte rdata[2];
  delay(30);
  if(!readMemory(deviceaddress, eeaddress, rdata)){
    return false;
  }
  return (rdata[0] == wdata[0] && rdata[1] == wdata[1]);
}



bool readDeviceSettings(uint8_t deviceaddress){
  byte r[2];
  for(uint8_t i = 0; i < 4; i++){
    if(!readMemory(deviceaddress, i, device_settings[i])){
      return false;
    }
  }
}

bool checkDefaultSettings(){
  for(uint8_t i = 0; i < 4; i++){
    if((device_settings[i][0] & default_settings_mask[i][0]) != default_settings[i][0])
      return false;
    if((device_settings[i][1] & default_settings_mask[i][1]) != default_settings[i][1])
      return false;
  }
  return true;
}
bool writeDefaultSettings(uint8_t deviceaddress){
  bool ok = true;
  for(uint8_t i = 0; i < 4; i++){
    ok = true;
    if((device_settings[i][0] & default_settings_mask[i][0]) != default_settings[i][0]){
      device_settings[i][0] = (device_settings[i][0] & ~default_settings_mask[i][0]) | default_settings[i][0];
      ok = false;
    }
    if((device_settings[i][1] & default_settings_mask[i][1]) != default_settings[i][1]){
      device_settings[i][1] = (device_settings[i][1] & ~default_settings_mask[i][1]) | default_settings[i][1];
      ok = false;
    }
    if(!ok){
      if(!writeMemoryCheck(deviceaddress, i, device_settings[i])){
        return false;
      }
    }
  }
  return true;
}

bool readDeviceStatus(uint8_t deviceaddress){
  byte r[2];
  
  if(!readMemory(deviceaddress, 32, r))
    return false;
  device_status.data_valid = ~((r[0] >> 7) & 0b1); // 0 = data valid
  device_status.rel_pos = (r[0] & 0xf) << 8 | r[1];
  
  if(!readMemory(deviceaddress, 33, r))
    return false;
  device_status.data_valid = ~((r[0] >> 7) & 0b1); // 0 = data valid
  device_status.abs_pos = (r[0] & 0xf) << 8 | r[1];
  
  if(!readMemory(deviceaddress, 34, r))
    return false;
  device_status.magnet_too_far = (r[0] >> 6) & 0b1;
  device_status.magnet_too_close = (r[0] >> 5) & 0b1;
  
  if(!readMemory(deviceaddress, 35, r))
    return false;
  device_status.AGC_gain = (r[0] >> 4) & 0xf;
  device_status.tacho_overflow = (r[0] >> 2) & 0b1;
  device_status.tacho = (r[0] & 0b11) << 8 | r[1];

  return true;
}



void displayDeviceProperties(){
  Serial.println("-------------------------------");
  Serial.print("Rotary Encoder on addr: ");
  Serial.println(device_addr);
  Serial.print("Default settings:       ");
  Serial.println(checkDefaultSettings() ? "OK" : "NOT APPLIED");
  Serial.print("Positive Rotation:      ");
  Serial.println((device_settings[1][0] & 0b10000) ? "Counter-Clockwise" : "Clockwise");
  Serial.print("Zero Value:             ");
  Serial.println((uint16_t) (((device_settings[1][0] & 0b1111) << 8) | device_settings[1][1]));
  Serial.println("-------------------------------");
  Serial.println();
  Serial.flush();
}

void displayDeviceStatus(){
  Serial.println("-------------------------------");
  Serial.print("Rotary Encoder status: ");
  Serial.println(device_status.data_valid ? "(valid)" : "(invalid data)");
  Serial.print("Magnet:            ");
  if(device_status.magnet_too_close || device_status.magnet_too_far){
    if(device_status.magnet_too_close)
      Serial.println("TOO CLOSE  ");
    if(device_status.magnet_too_far)
      Serial.println("TOO FAR  ");
    Serial.println("");
  }else{
    Serial.println("OK");
  }
  Serial.print("Current AGC gain:  ");
  Serial.println(device_status.AGC_gain);
  Serial.print("Rotation Rel:      ");
  Serial.println(device_status.rel_pos);
  Serial.print("Rotation Abs:      ");
  Serial.println(device_status.abs_pos);
  Serial.print("Tacho Value:       ");
  Serial.println(device_status.tacho);
  if(device_status.tacho_overflow)
    Serial.print(" (overflow)");
  Serial.println("-------------------------------");
  Serial.println();
  Serial.flush();
}

