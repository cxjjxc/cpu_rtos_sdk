/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023-2024, ArtInChip Technology Co., Ltd
 *
 * Authors:
 *   dwj <weijie.ding@aic.com>
 */
#include <rtconfig.h>

#define DDRC_BASE               0x98400000
#define DDR_PHY_BASE            0x98500000
#define GTC_CNTVL               0x89050008
#define GTC_CNTVH               0x8905000C
#define PRCM_SW_VDD11_CTL       0x88000070
#define PRCM_C908_VDD11_CTL     0x88000074
#define PRCM_DDR_WAKEUP_STATUS  0x88000108

.macro delay_200us
    li      s0, GTC_CNTVL
    li      s1, GTC_CNTVH
    lw      s2, (s0)     //start value 32bit low
    lw      s3, (s1)     //start value 32bit high
1:
    lw      s4, (s0)     //current value 32bit low
    lw      s5, (s1)     //current value 32bit high
    sub     s6, s4, s2   //s6 save 32bit low of sub
    sltu    s7, s4, s6   //s7 save carry
    sub     s8, s5, s3   //s8 save 32bit high of sub
    sub     s8, s8, s7   //s8 sub carry
    bnez    s8, 2f
    li      s4, 800      //GTC frequency is 4000000Hz, 200us counter 800
    bltu    s6, s4, 1b
2:
.endm

    .section .entry, "ax", %progbits
    .align  3
    .option pic
    .global aic_suspend_resume
aic_suspend_resume:
    //step1: DDR enter self refresh
    //ddr self-refresh flow
    li      t0, DDRC_BASE
    li      t1, DDR_PHY_BASE

    //disable DDR port
    li      t2, 0
    li      t3, 0
    li      t5, 5
    addi    t0, t0, 0x490
port_close_loop:
    add     t0, t0, t2
    sw      zero, (t0)
    addi    t2, t2, 0xb0
    addi    t3, t3, 1
    bltu    t3, t5, port_close_loop

    //enter self refresh
    li      t0, DDRC_BASE
    lw      t1, 0x30(t0)
    ori     t1, t1, 0x21
    sw      t1, 0x30(t0)
#ifndef FPGA_BOARD_ARTINCHIP
    delay_200us

    //step2: C908 VDD11 power down
    li      t0, PRCM_C908_VDD11_CTL
    lw      t1, (t0)
    li      t2, 0xFFFEFFFF
    and     t1, t1, t2
    li      t3, 0x27000000
    or      t1, t1, t3          //C908 VDD11 key
    sw      t1, (t0)            //clear PMU_RESET
    delay_200us
    li      t2, 0x8000
    or      t1, t1, t2
    sw      t1, (t0)            //set ISO_ENABLE
    delay_200us
PD_C908_check:
    lw      t1, (t0)
    li      t2, 0x100
    and     t3, t1, t2
    bnez    t3, PD_C908_check       //check POWER_SWITCH_D_STATUS

    //step3: VDD11 SW power down
    //Power down flow
    li      t0, PRCM_SW_VDD11_CTL
    lw      t1, (t0)
    li      t2, 0xFFFEFFFF
    and     t1, t1, t2
    li      t3, 0x27000000
    or      t1, t1, t3          //VDD11 SW key
    sw      t1, (t0)            //clear PMU_RESET
    delay_200us
    li      t2, 0x8000
    or      t1, t1, t2
    sw      t1, (t0)            //set ISO_ENABLE
    delay_200us
    li      t2, 0xFFFFBFFF
    and     t1, t1, t2
    sw      t1, (t0)            //clear DDR_RETENTION_EN
    delay_200us
    li      t2, 0xFFFFDFFF
    and     t1, t1, t2
    sw      t1, (t0)            //clear SRAM_RETENTION_EN_PRE
    delay_200us
    li      t2, 0xFFFFEFFF
    and     t1, t1, t2
    sw      t1, (t0)            //clear SRAM_RETENTION_EN
    delay_200us
PD_PSD_check:
    lw      t1, (t0)
    li      t2, 0x200
    and     t3, t1, t2
    bnez    t3, PD_PSD_check       //check POWER_SWITCH_D_STATUS
PD_PSA_check:
    lw      t1, (t0)
    li      t2, 0x100
    and     t3, t1, t2
    bnez    t3, PD_PSA_check       //check POWER_SWITCH_A_STATUS

    li      t2, 0xFFFFFF7F
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit7
    delay_200us
    li      t2, 0xFFFFFFBF
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit6
    delay_200us
    li      t2, 0xFFFFFFDF
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit5
    delay_200us
    li      t2, 0xFFFFFFEF
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit4
    delay_200us
    li      t2, 0xFFFFFFF7
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit3
    delay_200us
    li      t2, 0xFFFFFFFB
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit2
    delay_200us
    li      t2, 0xFFFFFFFD
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit1
    delay_200us
    li      t2, 0xFFFFFFFE
    and     t1, t1, t2
    sw      t1, (t0)            //clear POWER_SWITCH_EN[7:0] bit0
    delay_200us
#endif
    //enter wfi
    wfi

#ifndef FPGA_BOARD_ARTINCHIP
    //step1: VDD11 SW power up
    li      t0, PRCM_SW_VDD11_CTL
    lw      t1, (t0)
    li      t2, 0x27000001
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit0 with VDD11 key
    delay_200us
    li      t2, 0x2
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit1
    delay_200us
    li      t2, 0x4
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit2
    delay_200us
    li      t2, 0x8
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit3
    delay_200us
    li      t2, 0x10
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit4
    delay_200us
    li      t2, 0x20
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit5
    delay_200us
    li      t2, 0x40
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit6
    delay_200us
    li      t2, 0x80
    or      t1, t1, t2
    sw      t1, (t0)            //set POWER_SWITCH_EN[7:0] bit7
    delay_200us
PU_PSA_check:
    lw      t1, (t0)
    li      t2, 0x100
    and     t3, t1, t2
    beqz    t3, PU_PSA_check       //check POWER_SWITCH_A_STATUS
PU_PSD_check:
    lw      t1, (t0)
    li      t2, 0x200
    and     t3, t1, t2
    beqz    t3, PU_PSD_check       //check POWER_SWITCH_A_STATUS

    li      t2, 0x1000
    or      t1, t1, t2
    sw      t1, (t0)            //set SRAM_RETENTION_EN
    delay_200us
    li      t2, 0x2000
    or      t1, t1, t2
    sw      t1, (t0)            //set SRAM_RETENTION_EN_PRE
    delay_200us
    li      t2, 0x4000
    or      t1, t1, t2
    sw      t1, (t0)            //set DDR_RETENTION_EN
    delay_200us
    li      t2, 0xFFFF7FFF
    and     t1, t1, t2
    sw      t1, (t0)            //clear ISO_ENABLE
    delay_200us
    li      t2, 0x10000
    or      t1, t1, t2
    // set PMU_RESET, SESS will boot
    sw      t1, (t0)            //set PMU_RESET
    delay_200us

    //step2: C908 VDD11 power up
    li      t0, PRCM_C908_VDD11_CTL
PU_C908_check:
    lw      t1, (t0)
    li      t2, 0x100
    and     t3, t1, t2
    beqz    t3, PU_C908_check

    li      t2, 0xFFFF7FFF
    and     t1, t1, t2
    sw      t1, (t0)            //clear ISO_ENABLE
    delay_200us
    li      t2, 0x10000
    or      t1, t1, t2
    sw      t1, (t0)            //set PMU_RESET
    delay_200us
#endif

    //step3: DDR exit self-refresh
    li      t0, DDRC_BASE
    li      t1, DDR_PHY_BASE

    lw      t1, 0x30(t0)
    li      t2, 0xDE
    and     t1, t1, t2
    sw      t1, 0x30(t0) //exit self-refresh

    li      t2, 0
    li      t3, 0
    li      t4, 1
    li      t5, 5
    addi    t0, t0, 0x490
port_open_loop:
    add     t0, t0, t2
    sw      t4, (t0)
    addi    t2, t2, 0xb0
    addi    t3, t3, 1
    bltu    t3, t5, port_open_loop

    .long 0x0100000b    //icache.iall
    .long 0x0010000b    //dcache.call
    ret
aic_suspend_resume_end:

    .data
    .align  3
    .global aic_suspend_resume_size
aic_suspend_resume_size:
    .word   aic_suspend_resume_end - aic_suspend_resume

