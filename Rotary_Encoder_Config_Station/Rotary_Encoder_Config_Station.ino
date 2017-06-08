#include <Wire.h>
#include <avr/wdt.h>
#include "Definitions.h"



void setup() {
  
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  // Reset the Arduino if it hangs for more than 2s
  wdt_enable(WDTO_2S);
  
  Serial.println();
  Serial.println(F("Reboot, please reconnect any devices (I2C might be blocked)"));
  Serial.println();
  Serial.println(F("AM4096 Rotary Encoder Config Utility"));
  Serial.println();
}


byte r[2];
bool checkDeviceAvailable(int addr){
    return readMemory(addr, 1, r); // Read a register (fast)
}

void searchAddressSpace(){
  byte r[2];
  all_devices_num = 0;
  for(uint8_t i=0; i < 128; i++){ // search all possible addresses
    if(checkDeviceAvailable(i)){
      all_devices_addr[all_devices_num] = i;
      all_devices_num++;
      if(all_devices_num >= all_devices_num_max)
        return;
    }
  }
}




uint8_t device_addr = 0;

/* Modes:
 * Not connected -> Will keep searching
 * Multi connected -> Will show list and check list until one of them disconnects
 * Single connected -> Will show single until disconnect
 */


void loop() {
  wdt_reset();

  // No device connected
  if(all_devices_num == 0){
    Serial.println(F("Scanning..."));
    searchAddressSpace();
    device_list_displayed = false;
    device_settings_read = false;
    if(all_devices_num == 0){
      delay(1000);
    }
  }

  // Multi device connected
  else if(device_addr > 127){
    if(all_devices_num == 1){
      // Only one device => connect directly
      device_addr = all_devices_addr[0];
      Serial.print(F("Device found at addr: "));
      Serial.println(device_addr);
    }else{
      // Multiple devices => Enable list mode
      for(uint8_t i = 0; i < all_devices_num; i++){
        if(!checkDeviceAvailable(all_devices_addr[i])){
          Serial.println(F("Connection lost to at least one device"));
          Serial.println();
          all_devices_num = 0;
          break;
        }
      }
      if(all_devices_num > 0){
        if(!device_list_displayed){
          device_list_displayed = true;
          Serial.print(F("Multiple devices found: "));
          Serial.println(all_devices_num);
          Serial.println(F("On Addresses:"));
          for(uint8_t i = 0; i < all_devices_num; i++){
            if(i != 0){
              Serial.print(',');
            }
            Serial.print(all_devices_addr[i]);
          }
          Serial.println();
          Serial.println();
          Serial.println(F("Waiting for input... (h for help)"));
        }
        
        readCommand(false);
        delay(1);
      }
    }
  }

  // Single device connected
  else{
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

      if(!device_settings_read){
        readDeviceSettings(device_addr);
        device_settings_read = true;
        displayDeviceProperties();
        Serial.println(F("Waiting for input... (h for help)"));
      }

      readCommand(true);
      delay(1);

    }else{
      device_addr = 255;
      all_devices_num = 0;
      Serial.println(F("Connection lost"));
      Serial.println();
    }
  }
}


void readCommand(bool single){
  while (Serial.available()) {
      char c = Serial.read();
      if(c == '\n'){
        line = true;
        break;
      }
      commandIn += c;
  }
  if(line){
    line = false;
    Serial.println();
    Serial.print(">  ");
    Serial.println(commandIn);

    if(! (single ? parseSingleCommand(commandIn) : parseListCommand(commandIn))){
      Serial.println(F("Unknown command"));
      Serial.println(F("type h for help"));
    }

    delay(1000);

    
    commandIn = "";

  }
}


