## wdt_lib v1.0

This is wdt_lib library for using the watchdog functionality in Project A-tirma 

Copyright (C) 2024 Antonio C. Domínguez Brito (<antonio.dominguez@ulpgc.es>). División de Robótica y Oceanografía Computacional (<http://www.roc.siani.es>) and Departamento de Informática y Sistemas (<http://www.dis.ulpgc.es>). Universidad de Las Palmas de Gran  Canaria (ULPGC) (<http://www.ulpgc.es>).
  
### 0. License 


The wdt_lib library is an open source project which is openly available under the GNU General Public License (GPL) license.

### 1. Introduction

This is a C++ library abstracting the use of the hardware watchdog in Project A-Tirma, specifically aimed to the Arduino MKR electronic prototyping platform.

The library allows to create blocks of code which are protected by a watchdog (*watchdog-guarded*). This is illustrated with the following portion of code:


```
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

```

In this code we are establishing a block of code guarded with a watchdog time of 10 seconds, meaning that, if this code takes more than 10 seconds to complete, the watchdog will reset the system. Otherwise it will continue running normally past the watchdog-guarded block of code.

### 2. Examples

On directory *examples* you will find an illustrative example of how to use the library.

### 3. Library users

In this section we would like to enumerate users using the library in their own projects and developments. So please, if you are using the library, drop us an email indicating in what project or development you are using it.

The list of users/projects goes now:

1. **Project:** Project A-Tirma. **User**: División de Robótica y Oceanografía Computacional (<http://www.roc.siani.es>). **Description**: The library was a specific development for this project. 

### 10. Feedback & Suggestions

Please be free to send me any comment, doubt of use, or suggestion in relation to wdt_lib.


