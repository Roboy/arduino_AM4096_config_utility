
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

bool _writeMemory(uint8_t deviceaddress, uint8_t eeaddress, byte* wdata)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write(eeaddress); // LSB
  Wire.write(wdata, 2); // LSB
  if(Wire.endTransmission(true) != 0){
    return false;
  }
  return true;
}

bool writeMemory(int deviceaddress, unsigned int eeaddress, byte* wdata)
{
  if(!_writeMemory(deviceaddress, eeaddress, wdata)){
    return false;
  }
  byte rdata[2];
  delay(30);
  if(!readMemory(deviceaddress, eeaddress, rdata)){
    return false;
  }
  return (rdata[0] == wdata[0] && rdata[1] == wdata[1]);
}



void readDeviceProperties(int deviceaddress){
  
}

void displayDeviceProperties(){
  
}

