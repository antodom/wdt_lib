#include <Arduino.h>

#include "wdt_lib.hpp"

namespace mkr=arduino_mkr;

void setup() 
{
  Serial.begin(9600);

  Serial.print("["); 
  Serial.print(millis()); 
  Serial.println("] mkr_wdt_example: testing the watchdog"); 

  delay(5000);

  mkr::wdt::init(); 
}

void loop() 
{
  {
    mkr::wdt_guard guard(10000);

    uint32_t last_time=millis();
    while(millis()-last_time<5000)
    {
      Serial.print("["); Serial.print(millis()); Serial.print("]: first guard, "); 
      if(mkr::wdt::is_armed()) Serial.println("watchdog armed!");
      else Serial.println("watchdog disarmed");

      delay(1000);
    }
  }

  {
    mkr::wdt_guard guard(10000);

    uint32_t last_time=millis();
    while(millis()-last_time<10500)
    {
      Serial.print("["); Serial.print(millis()); Serial.print("]: second guard, "); 
      if(mkr::wdt::is_armed()) Serial.println("watchdog armed!");
      else Serial.println("watchdog disarmed");

      delay(1000);
    }
  }
}


