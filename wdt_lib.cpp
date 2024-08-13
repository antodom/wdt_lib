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

/* wdt_lib.cpp
 *
 * author: Antonio C. Domínguez Brito <adominguez@iusiani.ulpgc.es>
 * 
 * Description:  Watchdog lib used in project A-Tirma, implementation file.
 */

#include "wdt_lib.hpp"

#define WDT_LIB_CPP

namespace arduino_mkr
{
  volatile bool wdt::armed__=false;
  volatile uint32_t wdt::timeout_ms__=0;
  volatile uint32_t wdt::arm_time_ms__=0;

  void wdt::init()
  {
    // setting watchdog clock
    GCLK->GENDIV.reg=GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(clk_div__);
    GCLK->GENCTRL.reg= 
      GCLK_GENCTRL_ID(2) | 
      GCLK_GENCTRL_GENEN | 
      GCLK_GENCTRL_SRC_OSCULP32K | 
      GCLK_GENCTRL_DIVSEL;
  
    while(GCLK->STATUS.bit.SYNCBUSY);
    GCLK->CLKCTRL.reg = 
      GCLK_CLKCTRL_ID_WDT | 
      GCLK_CLKCTRL_CLKEN | 
      GCLK_CLKCTRL_GEN_GCLK2;
    while(GCLK->STATUS.bit.SYNCBUSY);
    
    // setting watchdog early-warning interrupt 
    NVIC_DisableIRQ(WDT_IRQn);
    NVIC_ClearPendingIRQ(WDT_IRQn);
    NVIC_SetPriority(WDT_IRQn,0); // highest priority
    NVIC_EnableIRQ(WDT_IRQn);
     
    // setting watchdog
    WDT->CONFIG.bit.PER=
      WDT_CONFIG_PER_16K_Val; // watchdog time at 16384 clock cycles (2 seconds)   
    while(WDT->STATUS.bit.SYNCBUSY);     
  
    WDT->CTRL.bit.WEN=
      0; // winddow mode disabled       
    while(WDT->STATUS.bit.SYNCBUSY);     
  
    WDT->EWCTRL.bit.EWOFFSET=
      WDT_EWCTRL_EWOFFSET_8K_Val; // interrupts each 8192 clock cycles (1 second)      
  
    WDT->INTENSET.bit.EW=
      1; // enabling early-warning interrupt       
  
    // enabling watchdog
    WDT->CTRL.bit.ENABLE=1;        
    while(WDT->STATUS.bit.SYNCBUSY);

    clear__();
  }

}

void WDT_Handler(void) 
{ 
  WDT->INTFLAG.bit.EW=
    1; // clearing early-warning interrupt flag     

  if(!arduino_mkr::wdt::armed__) arduino_mkr::wdt::clear__();
  else
  {
    if(millis()-arduino_mkr::wdt::arm_time_ms__>arduino_mkr::wdt::timeout_ms__) 
      arduino_mkr::wdt::reset__();
    else
      arduino_mkr::wdt::clear__();
  }
}

