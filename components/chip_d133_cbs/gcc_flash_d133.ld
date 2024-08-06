/*
 * Copyright (C) 2017-2024 Alibaba Group Holding Limited
 */

/*
 ******************************************************************************
                d13x Memory Layout
 ******************************************************************************

           配置(1)        配置(2)       配置(3)
          SRAM_S1 OFF   SRAM_S1 OFF    SRAM_S1 ON
              TCM ON        TCM OFF
0x30040000+----------+  +----------+ +----------+
          |   ITCM   |  |          | |          |
          |   128K   |  |          | |   TCM    |
0x30060000+----------+  |          | |    +     |
          |   DTCM   |  |          | | SRAM_S0  |
          |   128k   |  |  SRAM_S0 | |          |
0x30080000+----------+  |    1M    | |1M-sram_s1|
          |          |  |          | |   size   |
          |          |  |          | |          |
          |          |  |          | |          |
          |  SRAM_S0 |  |          | +----------+
          |   768K   |  |          |
          |          |  |          |
          |          |  |          |
          |          |  |          |
          |          |  |          |
0x3013FFFF+----------+  +----------+

0x3FF00000 ------------------------> +----------+
- sram_s1 size                       |  SRAM_S1 |
                                     |          |
                                     |128K/256K/|
                                     |384K/512K/|
                                     |640K/768K |
0x3FFFFFFF ------------------------> +----------+

0x40000000+----------+
          |          |
          |  PSRAM   |
          |  4M/8M   |
          |          |
          |          |
          +----------+
          |          |
          | FPGA Ext |
          |  PSRAM   |
          | 64M-psram|
          |     size |
          |          |
          |          |
          |          |
          |          |
          |          |
          |          |
0x43FFFFFF+----------+
 */

/******************************************************************************
 * @file     gcc_csky.ld
 * @brief    csky linker file
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
MEMORY
{
    BROM        : ORIGIN = 0x30000000                                             , LENGTH = 256K
    SRAM_S0     : ORIGIN = 0x30040000                                             , LENGTH = 0x100000 - 0x40000
    PSRAM_CMA   : ORIGIN = 0x40000000                                             , LENGTH = 0x800000
}

__min_heap_size = 0x1000;
PROVIDE (__ram_end  = 0x30140000);
PROVIDE (__heap_end = __ram_end);

REGION_ALIAS("REGION_TEXT",    SRAM_S0);
REGION_ALIAS("REGION_RODATA",  SRAM_S0);
REGION_ALIAS("REGION_DATA",    SRAM_S0);
REGION_ALIAS("REGION_BSS",     SRAM_S0);

ENTRY(Reset_Handler)
SECTIONS
{
 .text : {
  . = ALIGN(0x4) ;
  __stext = . ;
  KEEP(*startup.o(*.text))
  KEEP(*startup.o(*.vectors))
  KEEP(*vectors.o(*.text))
  KEEP(*(.text.entry))
  *(.text*)
  *(.gnu.warning)
  *(.stub)
  *(.gnu.linkonce.t*)
  *(.glue_7t)
  *(.glue_7)
  *(.jcr)
  KEEP (*(.init))
  KEEP (*(.fini))
  . = ALIGN (0x4) ;
  PROVIDE(__ctbp = .);
  *(.call_table_data)
  *(.call_table_text)
  . = ALIGN(0x10) ;
  __etext = . ;
 } > REGION_TEXT
 .rodata : {
  . = ALIGN(0x4) ;
  __srodata = .;
  *(.rdata)
  *(.rdata*)
  *(.rdata1)
  *(.rdata.*)
  *(.rodata*)
  *(.srodata*)
  . = ALIGN(0x4) ;
  __init_array_start = .;
  __ctors_start__ = .;
  KEEP (*(SORT(.init_array.*)))
  KEEP (*(.init_array))
  __init_array_end = .;
  __ctors_end__ = .;

  __fini_array_start = .;
  __dtors_start__ = .;
  KEEP (*(SORT(.fini_array.*)))
  KEEP (*(.fini_array))
  __fini_array_end = .;
  __dtors_end__ = .;
  . = ALIGN(0x4) ;

  __ctor_start__ = .;
  KEEP (*(SORT(.ctors.*)))
  KEEP (*(.ctors))
  __ctor_end__ = .;
  KEEP (*(SORT(.dtors.*)))
  KEEP (*(.dtors))
  __dtor_end__ = .;
  . = ALIGN(0x4) ;
/*****************************************/
  /* section information for finsh shell */
  . = ALIGN(0x4);
  __fsymtab_start = .;
  KEEP(*(FSymTab))
  __fsymtab_end = .;
  . = ALIGN(0x4);
  __vsymtab_start = .;
  KEEP(*(VSymTab))
  __vsymtab_end = .;
  . = ALIGN(0x4);

  /* section information for initial. */
  __rt_init_start = .;
  KEEP(*(SORT(.rti_fn*)))
  __rt_init_end = .;
  . = ALIGN(0x4) ;

  /* section information for at utest */
  __rt_utest_tc_tab_start = .;
  KEEP(*(UtestTcTab))
  __rt_utest_tc_tab_end = .;
  . = ALIGN(0x4);

  /* section information for at server */
  . = ALIGN(0x4);
  __rtatcmdtab_start = .;
  KEEP(*(RtAtCmdTab))
  __rtatcmdtab_end = .;
  . = ALIGN(0x4);

  /* section information for modules */
  . = ALIGN(0x4);
  __rtmsymtab_start = .;
  KEEP(*(RTMSymTab))
  __rtmsymtab_end = .;

  /* section information for uPRC */
  . = ALIGN(0x4);
  __uRPCSvcTab_start = .;
  KEEP(*(uRPCSvcTab))
  __uRPCSvcTab_end = .;

  /* section information for var export */
  . = ALIGN(0x4);
  __ve_table_start = .;
  KEEP(*(SORT(*.VarExpTab.*)))
  __ve_table_end = .;
/*****************************************/
/************** added drivers **************/
  _cli_region_begin = .;
  KEEP(*(CliRegion))
  . = ALIGN(0x4);
  _cli_region_end = .;

  __core_driver_start__ = .;
  KEEP(*(.core_driver_entry))
  . = ALIGN(0x4);
  __core_driver_end__ = .;

  __bus_driver_start__ = .;
  KEEP(*(*.bus_driver_entry))
  __bus_driver_end__ = .;

  __early_driver_start__ = .;
  KEEP(*(*.early_driver_entry))
  __early_driver_end__ = .;

  __vfs_driver_start__ = .;
  KEEP(*(*.vfs_driver_entry))
  __vfs_driver_end__ = .;

  __level0_driver_start__ = .;
  KEEP(*(*.level0_driver_entry))
  __level0_driver_end__ = .;

  __level1_driver_start__ = .;
  KEEP(*(*.level1_driver_entry))
  __level1_driver_end__ = .;

  __level2_driver_start__ = .;
  KEEP(*(*.level2_driver_entry))
  __level2_driver_end__ = .;

  __level3_driver_start__ = .;
  KEEP(*(*.level3_driver_entry))
  __level3_driver_end__ = .;

  __post_driver_start__ = .;
  KEEP(*(*.post_driver_entry))
  __post_driver_end__ = .;
/************** end of drivers *********/
  . = ALIGN(0x4) ;
  __erodata = .;
  __rodata_end__ = .;
 } > REGION_RODATA
 .data : {
  . = ALIGN(0x4) ;
  __sdata = . ;
  __data_start__ = . ;
  data_start = . ;
  *(.got.plt)
  *(.got)
  *(.gnu.linkonce.r*)
  *(.data*)
  *(.gnu.linkonce.d*)
  *(.gcc_except_table*)
  __start_init_call = .;
  *(.initcall.init)
  __stop_init_call = .;
  __start_cmd = .;
  *(.bootloaddata.cmd)
  . = ALIGN(0x4) ;
  __stop_cmd = .;
  __global_pointer$ = .;
  *(.sdata)
  *(.sdata.*)
  *(.sdata2.*)
  *(.gnu.linkonce.s.*)
  *(__libc_atexit)
  *(__libc_subinit)
  *(__libc_subfreeres)
  *(.note.ABI-tag)
  __edata = .;
  __data_end__ = .;
  . = ALIGN(0x4) ;
 } > REGION_DATA AT > REGION_RODATA
 ._ram_code : {
  . = ALIGN(0x4) ;
  __ram_code_start__ = .;
  *(.ram.code*)
  . = ALIGN(0x4) ;
  __ram_code_end__ = .;
 } > REGION_DATA AT > REGION_RODATA
 .bss : {
  . = ALIGN(0x4) ;
  __sbss = ALIGN(0x4) ;
  __bss_start__ = . ;
  *(.dynsbss)
  *(.sbss)
  *(.sbss.*)
  *(.scommon)
  *(.dynbss)
  *(.bss*)
  *(COMMON)
  . = ALIGN(0x4) ;
  __ebss = . ;
  __bss_end__ = .;
  __end = . ;
  end = . ;
 } > REGION_BSS AT > REGION_BSS
 ._user_heap (NOLOAD): {
  . = ALIGN(0x4) ;
  *(.stack*)
  . = ALIGN(0x4) ;
  __heap_start = .;
  . += __min_heap_size;
  . = ALIGN(0x4) ;
 } > REGION_BSS AT > REGION_BSS
}
