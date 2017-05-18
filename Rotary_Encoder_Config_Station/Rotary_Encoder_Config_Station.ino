#include <Wire.h>
#include "Definitions.h"



void setup() {
  
  Wire.begin();
  //Wire.setClock(400000);
  Serial.begin(115200);
  
}


byte r[2];
bool checkDeviceAvailable(int addr){
    return readMemory(addr, 1, r); // Read a register (fast)
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



uint8_t device_addr = 0;

void loop() {

  // No device connected
  if(device_addr > 127){
    delay(500);
    Serial.println("Searching");
    device_addr = searchAddressSpace();
  }else{
    if(checkDeviceAvailable(device_addr)){
      // Device connected
      byte data[2];
      readMemory(0, 33, data);
      bool dataOK = (data[0] >> 7) & 0b1;
      int absPos = data[0] << 8 | data[1];
      readMemory(0, 34, data);
      bool tooFar = (data[0] >> 6) & 0b1;
      bool tooClose = (data[0] >> 5) & 0b1;
      Serial.print("data ok: ");
      Serial.println(dataOK);
      Serial.print("tooFar: ");
      Serial.println(tooFar);
      Serial.print("tooClose: ");
      Serial.println(tooClose);
      Serial.print("absPos: ");
      Serial.println(absPos);
    }else{
      device_addr = 255;
      Serial.println("Connection lost");
    }
    delay(1000);
  }
}


