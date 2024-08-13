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

/* wdt_lib.hpp
 *
 * author: Antonio C. Domínguez Brito <adominguez@iusiani.ulpgc.es>
 * 
 * Description:  Watchdog lib used in project A-Tirma, header file.
 */

#ifndef WDT_LIB_HPP
  #define WDT_LIB_HPP

  #include <Arduino.h>

  namespace arduino_mkr
  {

    #ifndef __SAMD21G18A__
      #error "__SAMD21G18A__ not defined! This library should be used with an Arduino MKR board!"
    #endif

    class wdt
    {
      public:

        static void init();

        static bool is_armed() { return armed__; }

        static void arm(uint32_t timeout_ms = 0) 
        {  
          interrupt_lock__ lock; 
          armed__=true; 
          arm_time_ms__=millis(); 
          timeout_ms__=timeout_ms;
        }

        static void disarm() { interrupt_lock__ lock; armed__=false; clear__(); }

      private:

        class interrupt_lock__
        {
          public:

            interrupt_lock__() { noInterrupts(); }
            ~interrupt_lock__() { interrupts(); }
        };

        // watchdog clock at 1024 Hz 
        // See: SAMD21 Family Data Sheet, GCLK - Generic Clock Controller 
        static constexpr uint8_t clk_div__=1; // watchdog frequency of 8192 Hz
        
        static volatile bool armed__;
        static volatile uint32_t arm_time_ms__;
        static volatile uint32_t timeout_ms__;

        static void clear__()
        {
          WDT->CLEAR.bit.CLEAR=
            WDT_CLEAR_CLEAR_KEY_Val; // clearing watchdog
          while(WDT->STATUS.bit.SYNCBUSY); 
        }

        static void reset__()
        {
          WDT->CLEAR.bit.CLEAR=
            WDT_CLEAR_RESETVALUE;// reset the system 
          while(WDT->STATUS.bit.SYNCBUSY); 
        }

        friend void ::WDT_Handler(void); 
    };

    class wdt_guard
    {
      public:

        wdt_guard(uint32_t timeout_ms) { Serial.println(__func__); wdt::arm(timeout_ms); }
        ~wdt_guard() { Serial.println(__func__); wdt::disarm(); }
    };
  }

#endif // WDT_LIB_HPP
