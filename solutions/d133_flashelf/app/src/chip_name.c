/*============================================================================
 * Name        : chip_name.c
 * Author      : $(UserName)
 * Copyright   : Your copyright notice
 * Description : System Initialization before main
 * 1 CPU initialization
 * 2 memory & IO base address initialization
 * 3 flash size & address initialization
 * 4 interrupt resource initialization for the soc
 *============================================================================
 */
#include <string.h>

#if 0
/**
 * Globl symbols in ckcpu.ld 
 */
extern char __e_ram_bss_1[];
extern char __s_ram_bss_1[];

static void MemoryInit( void ) 
{
	  /* zero the bss 
	   */
	  if( __e_ram_bss_1 - __s_ram_bss_1 ) 
	  {
		memset( __s_ram_bss_1, 0x00, ( __e_ram_bss_1 - __s_ram_bss_1 ));
	  }
}


/**
 * @brief entry of whole chip initialization
 * Since here SystemInit is a weak symbol, any vendor can override this symbol on its own wishes.
 */
__attribute__((weak)) void  SystemInit(void) 
{
	/**
	 * Initial the bss section and data decompression from LMA to VMA
	 */
	MemoryInit();
	
	/**
	 * TODO: initial IO, memory, flash... 
	 */
	
    return;
}

__attribute__((weak)) void __main()
{
	SystemInit();
	// got to main 
	extern int main(void);
	main();
	// loop here;
	while(1);
}

void __attribute__((weak)) handle_trap(unsigned int cause, unsigned int epc, unsigned int regs[32])
{

}

#endif