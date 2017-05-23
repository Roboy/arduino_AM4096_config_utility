


bool parseListCommand(String commandIn){

  // help
  if(commandIn == "h"){
    Serial.println(F("List device commands available: (line end with \\n)"));
    Serial.println(F("h       -- Show this help"));
    Serial.println(F("l       -- Refresh this list"));
    Serial.println(F("list    -- Print a detailed list about the connected devices"));
    Serial.println(F("c <val> -- Connect to device at address <val>"));
  }
  
  // trigger reload of device list
  else if(commandIn == "l"){
    all_devices_num = 0;
  }

  // display detailed device list
  else if(commandIn == "list"){
    Serial.println();
    displayDevicePropertiesTableHeader();
    for(uint8_t i = 0; i < all_devices_num; i++){
      readDeviceSettings(all_devices_addr[i]);
      displayDevicePropertiesTableRow(all_devices_addr[i]);
    }
    Serial.println();
  }

  // connect to address
  else if(commandIn.startsWith("c ")){
    uint8_t addr = commandIn.substring(2).toInt() & 0b01111111; // 7 bit value max
    bool found = false;
    for(uint8_t i = 0; i < all_devices_num; i++){
      if(all_devices_addr[i] == addr){
        device_addr = all_devices_addr[i];
        Serial.print(F("Connected to device at addr: "));
        Serial.println(device_addr);
        found = true;
        break;
      }
    }
    if(!found){
      Serial.println(F("Given address does not correspond to a known device"));
    }
  }

  
  else{
    return false;
  }
  return true;
}
