/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         EXCEPTION VECTORS
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : app_vect-v5.c
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

#include  <ctype.h>


#include "mytypes.h"
#include "adc_sampler.h"
#include "time.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define DEF_TRUE 1

static INT_U8 u08Status = 0;


/*
*********************************************************************************************************
*                                            App_NMI_ISR()
*
* Description : Handle Non-Maskable Interrupt (NMI).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) Since the NMI is not being used, this serves merely as a catch for a spurious
*                   exception.
*********************************************************************************************************
*/

void  App_NMI_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}

/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  App_Fault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_BusFault_ISR()
*
* Description : Handle bus fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  App_BusFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                          App_UsageFault_ISR()
*
* Description : Handle usage fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  App_UsageFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_MemFault_ISR()
*
* Description : Handle memory fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  App_MemFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle spurious interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  App_Spurious_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle PendSV interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntPendSVHandler (void)
{
    while (DEF_TRUE) {
        ;
    }
}



/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle Tick ISR interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntSysTickHandler (void)
{
    while (DEF_TRUE) {
        ;
    }
}

/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle Tick tmr1 interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntHandlerTIM1 (void)
{
   /* trigger time */
   Time_IrqSet100MS();
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle Tick tmr2 interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntHandlerTIM2 (void)
{

}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle Tick EXTI Line0 interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntEXTILine0 (void)
{

   /* if extern trigger occure while the dma irq
      the wrong timer restart will prevent by the u08Status byte */
   if(u08Status == 0)
   {

   }
   else
   {
      u08Status = 0;
   }
}

/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle Tick EXTI Line1 interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntEXTILine1 (void)
{

   /* if extern trigger occure while the dma irq
      the wrong timer restart will prevent by the u08Status byte */
   if(u08Status == 0)
   {

   }
   else
   {
      u08Status = 0;
   }
}

/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle Tick EXTI Line0 interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  IntDMAChannel2(void)
{
   /* signal the dma is ready, a pending ext irq will not start
      the timer again when u08Status = 1 */
   u08Status = 1;

   /* stop timer */
   Adc_SamplerStop();
}

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  EXCEPTION / INTERRUPT VECTOR TABLE
*
* Note(s) : (1) The Cortex-M3 may have up to 256 external interrupts, which are the final entries in the
*               vector table.  The STM32 has 60 external interrupt vectors.
*********************************************************************************************************
*/
