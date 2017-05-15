#include <Wire.h>
#include "Definitions.h"



void setup() {
  
  Wire.begin();
  Serial.begin(115200)
  
}


byte r[2];
bool checkDeviceAvailable(int addr){
    return readMemory(addr, 32, r); // Read a register (fast)
}

uint8_t searchAddressSpace(){
  byte r[2];
  for(uint8_t i=0; i < 128; i++){ // search all possible addresses
    if(checkDeviceAvailable(i)){
      Serial.print("Device found at addr: ");
      Serial.println(i);
      return i;
    }
  }
  return 255; // invalid address (valid: 0 - 127)
}



uint8_t device_addr = 255;

void loop() {

  // No device connected
  if(device_addr > 127){
    delay(500);
    Serial.println("Searching");
    device_addr = searchAdressSpace();
  }else{
    if(checkDeviceAvailable(device_addr)){
      // Device connected

      displayDeviceProperties();
      
    }else{
      device_addr = 255;
      Serial.println("Connection lost");
    }
  }
}


