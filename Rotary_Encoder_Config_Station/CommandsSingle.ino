


bool parseSingleCommand(String commandIn){
  // Display status
  if(commandIn == "" || commandIn == "t"){
    if(!readDeviceStatus(device_addr)){
      Serial.println(F("Error reading device status"));
      device_settings_read = false;
    }else{
      displayDeviceStatus();
    }
  }

  // help
  else if(commandIn == "h"){
    Serial.println(F("Single device commands available: (line end with \\n)"));
    Serial.println(F("h       -- Show this help"));
    Serial.println(F("d       -- Disconnect / back to device list"));
    Serial.println(F("s       -- Display current device settings"));
    Serial.println(F("reg (-i)-- Display current raw register value (with comparison to default)"));
    Serial.println(F("def     -- Apply the default settings"));
    Serial.println(F("a <val> -- Change the Address"));
    Serial.println(F("z <val> -- Set a zero-value"));
    Serial.println(F("r       -- Flip the Rotation sign"));
    Serial.println(F("t       -- read the current state for testing"));
    Serial.println(F("(or blank)"));
  }
  
  // trigger reload of device settings
  else if(commandIn == "s"){
    device_settings_read = false;
  }

  // trigger reload of device list
  else if(commandIn == "d"){
    Serial.println(F("Disconnected from device"));
    Serial.println();
    device_addr = 255;
    all_devices_num = 0;
  }

  // flip rotation & write setting
  else if(commandIn == "r"){
    device_settings[1][0] ^= 0b10000;
    if(!writeMemoryCheck(device_addr, 1, device_settings[1])){
      Serial.println(F("Error flipping rotation"));
      device_settings_read = false;
    }
    else{
      Serial.print(F("Success: Rotation now "));
      Serial.println((device_settings[1][0] & 0b10000) ? F("Counter-Clockwise") : F("Clockwise"));
    }
  }

  // display raw register
  else if(commandIn.startsWith("reg")){
    char buf[30];
    // display raw register with additional info
    if(commandIn == "reg -i"){
      Serial.println(F("Current register values, default values, default mask:"));
      for(uint8_t i=0; i < 4; i++){
        sprintf(buf, "%02i: %02x %02x   %02x %02x   %02x %02x", i,
          device_settings[i][0], device_settings[i][1],
          default_settings[i][0], default_settings[i][1],
          default_settings_mask[i][0], default_settings_mask[i][1]);
        Serial.println(buf);
      }
    }else{
      Serial.println(F("Current register values:"));
      for(uint8_t i=0; i < 4; i++){
        sprintf(buf, "%02i: %02x %02x", i, device_settings[i][0], device_settings[i][1]);
        Serial.println(buf);
      }
    }
  }

  // write default settings
  else if(commandIn == "def"){
    if(!writeDefaultSettings(device_addr)){
      Serial.println(F("Error writing default settings"));
    }
    else{
      Serial.println(F("Success: Default settings written"));
    }
    device_settings_read = false;
  }

  // write zero value
  else if(commandIn.startsWith("z ")){
    uint16_t zero = commandIn.substring(2).toInt() & 0xfff; // 12 bit value max
    
    device_settings[1][0] = (device_settings[1][0] & 0xf0) | ((zero >> 8) & 0xf);
    device_settings[1][1] = zero & 0xff;
    if(!writeMemoryCheck(device_addr, 1, device_settings[1])){
      Serial.println(F("Error writing Zero Value rotation"));
      device_settings_read = false;
    }
    else{
      Serial.print(F("Success: Zero value now "));
      Serial.println(zero);
    }
  }

  // write address
  else if(commandIn.startsWith("a ")){
    uint8_t addr = commandIn.substring(2).toInt() & 0b01111111; // 7 bit value max
    
    device_settings[0][1] = (device_settings[0][1] & 0b10000000) | addr;
    if(!writeMemory(device_addr, 0, device_settings[0])){
      // No writeMemoryCheck here since address changes
      Serial.println(F("Error giving new address"));
      device_settings_read = false;
    }
    else{
      Serial.print(F("Success: Device address now "));
      Serial.println(addr);
      device_addr = addr;
    }
    delay(30);
  }

  
  else{
    return false;
  }
  return true;
}
