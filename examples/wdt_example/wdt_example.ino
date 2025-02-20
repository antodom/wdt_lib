/**
 ** wdt_lib library
 ** Copyright (C) 2024
 **
 **   Antonio C. Domínguez Brito <antonio.dominguez@ulpgc.es>
 **     División de Robótica y Oceanografía Computacional <www.roc.siani.es>
 **     and Departamento de Informática y Sistemas <www.dis.ulpgc.es>
 **     Universidad de Las Palmas de Gran  Canaria (ULPGC) <www.ulpgc.es>
 **  
 ** This file is part of the wdt_lib library.
 ** The wdt_lib library is free software: you can redistribute it and/or modify
 ** it under  the  terms of  the GNU  General  Public  License  as  published  by
 ** the  Free Software Foundation, either  version  3  of  the  License,  or  any
 ** later version.
 ** 
 ** The  wdt_lib library is distributed in the hope that  it  will  be  useful,
 ** but   WITHOUT   ANY WARRANTY;   without   even   the  implied   warranty   of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE.  See  the  GNU  General
 ** Public License for more details.
 ** 
 ** You should have received a copy  (COPYING file) of  the  GNU  General  Public
 ** License along with the wdt_lib library.
 ** If not, see: <http://www.gnu.org/licenses/>.
 **/

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


