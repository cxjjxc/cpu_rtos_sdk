/*
 * Copyright (C) 2014-2023 Alibaba Group Holding Limited
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdlib.h>
#include <stdint.h>
#include <csi_core.h>
#if defined(AOS_COMP_DEBUG) && (AOS_COMP_DEBUG > 0)
#include <debug/dbg.h>
#else
#define printk printf
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#if __riscv_xlen == 64
	#define portSTACK_TYPE			uint64_t
	#define portBASE_TYPE			int64_t
	#define portUBASE_TYPE			uint64_t
	#define portMAX_DELAY 			( TickType_t ) 0xffffffffffffffffUL
	#define portPOINTER_SIZE_TYPE 	uint64_t
#elif __riscv_xlen == 32
	#define portSTACK_TYPE	uint32_t
    #define portBASE_TYPE	int32_t
	#define portUBASE_TYPE	uint32_t
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#else
	#error Assembler did not define __riscv_xlen
#endif


typedef portSTACK_TYPE StackType_t;
typedef portBASE_TYPE BaseType_t;
typedef portUBASE_TYPE UBaseType_t;
typedef portUBASE_TYPE TickType_t;

/* Legacy type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short

/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
not need to be guarded with a critical section. */
#define portTICK_TYPE_IS_ATOMIC 1
/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#ifdef __riscv64
	#error This is the RV32 port that has not yet been adapted for 64.
	#define portBYTE_ALIGNMENT			16
#else
	#define portBYTE_ALIGNMENT			16
#endif
/*-----------------------------------------------------------*/

static inline void vPortEnableInterrupt( void )
{
    __enable_irq();
}

static inline void vPortDisableInterrupt( void )
{
    __disable_irq();
}

static inline portLONG SaveLocalPSR (void)
{
    portLONG flags = csi_irq_save();
    return flags;
}

static inline void RestoreLocalPSR (portLONG newMask)
{
    csi_irq_restore(newMask);
}

extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
extern __attribute__((naked)) void cpu_yeild(void);

#define portDISABLE_INTERRUPTS()                vPortDisableInterrupt()
#define portENABLE_INTERRUPTS()                 vPortEnableInterrupt()
#define portENTER_CRITICAL()                    vPortEnterCritical()
#define portEXIT_CRITICAL()                     vPortExitCritical()
#define portSET_INTERRUPT_MASK_FROM_ISR()       SaveLocalPSR()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(a)    RestoreLocalPSR(a)

#define portNOP() __asm volatile 	( " nop " )

extern portLONG ulCriticalNesting;
extern portLONG pendsvflag;

/* Scheduler utilities. */
extern void vPortYield( void );
#define portYIELD()                 vPortYield();

/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters ) __attribute__((noreturn))
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )
/*-----------------------------------------------------------*/

#define portEND_SWITCHING_ISR( xSwitchRequired )    do {    \
                                                            if( xSwitchRequired != pdFALSE )    \
                                                            {   \
                                                                portYIELD();    \
                                                            }   \
                                                    }while(0)

#define portYIELD_FROM_ISR( a )     portEND_SWITCHING_ISR( a )

extern int g_fr_next_sleep_ticks;
#define configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING(x) \
do                                                       \
{                                                        \
    g_fr_next_sleep_ticks = x;                           \
} while (0)

#if defined(CONFIG_DEBUG) && CONFIG_DEBUG
#define configASSERT( a )   do {if ((a)==0){printk("Assert : %s %d\r\n", __FILE__, __LINE__);while(1);}}while(0)
#else
#define configASSERT( a )   do {if ((a)==0){printk("Assert : %s %d\r\n", __FILE__, __LINE__);}}while(0)
#endif

/*-----------------------------------------------------------*/

#define portINLINE	__inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE inline __attribute__(( always_inline))
#endif

#define portMEMORY_BARRIER() __asm volatile( "" ::: "memory" )
/*-----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

