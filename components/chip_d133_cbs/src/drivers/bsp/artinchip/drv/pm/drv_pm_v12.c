/*
 * Copyright (c) 2022-2023, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: dwj <weijie.ding@artinchip.com>
 */

#include <stdio.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <aic_core.h>
#include <aic_drv.h>
#include <string.h>
#include <aic_osal.h>

uint64_t sleep_counter;
uint64_t resume_counter;

extern void aic_suspend_resume();
extern u32 aic_suspend_resume_size;
static void (*aic_suspend_resume_fn)();
extern size_t __sram_start;

#define PRCM_SW_VDD11_CTL           0x88000070
#define PRCM_C908_VDD11_CTL         0x88000074
#define PRCM_DDR_WAKEUP_STATUS      0x88000108
#define CMU_APB0_REG                0x98020120
#define CMU_APB2_REG                0x98020128

void aic_pm_enter_idle(void)
{
    __WFI();
}

#ifndef AIC_USING_SRAM
void aic_ddr_sr_code_on_ddr(void)
{
    rt_kprintf("aic_suspend_resume_size: %d\n", aic_suspend_resume_size);
    rt_kprintf("__sram_start: %x\n", (uint32_t)&__sram_start);

    rt_memcpy((void *)&__sram_start, aic_suspend_resume, aic_suspend_resume_size);
    aic_suspend_resume_fn = (void *)&__sram_start;
    aicos_icache_invalid();
    aicos_dcache_clean_invalid();
    aic_suspend_resume_fn();
}
#else
void aic_ddr_sr_code_on_sram(void)
{
    rt_kprintf("aic_suspend_resume_size: %d\n", aic_suspend_resume_size);
    aic_suspend_resume_fn = aic_suspend_resume;
    aic_suspend_resume_fn();
}
#endif

void aic_pm_enter_deep_sleep(void)
{
    rt_base_t level;
    uint32_t i, reg_val;
    uint8_t save_sp_clic_ie[MAX_IRQn] = {0};
    uint32_t cmu_pll_freq[8];
    uint32_t cmu_pll8_freq, cmu_apb0_freq, cmu_apb2_freq;

    level = rt_hw_interrupt_disable();
    /*
     * After VDD1.1 power domain reset, the CMU will also reset.
     * So save the CMU pll and bus register value before the VDD1.1 domain
     * power down.
     */
    for (i = 0; i < ARRAY_SIZE(cmu_pll_freq); i++)
        cmu_pll_freq[i] = hal_clk_get_freq(CLK_CS_PLL_FRA0 + i);

    /* TO DO  */
    RT_UNUSED(cmu_pll8_freq);

    cmu_apb0_freq = readl(CMU_APB0_REG);
    cmu_apb2_freq = readl(CMU_APB2_REG);

    /* change PRCM bus frequency to 24M */
    hal_clk_set_parent(CLK_AHB, CLK_24M);
    hal_clk_set_parent(CLK_APB0, CLK_24M);
    hal_clk_set_parent(CLK_AXI, CLK_24M);
    /* change SP cpu frequency to 24M */
    hal_clk_set_parent(CLK_CPU, CLK_24M);
    /* save the interrupt status of each peripheral  */
    for (i = 0; i < MAX_IRQn; i++)
    {
        save_sp_clic_ie[i] = (uint8_t)csi_vic_get_enabled_irq(i);
        if (save_sp_clic_ie[i])
            aicos_irq_disable(i);
    }

    /* Indicate DDR will enter self-refresh */
    reg_val = readl(PRCM_DDR_WAKEUP_STATUS);
    reg_val |= 1;
    writel(reg_val, PRCM_DDR_WAKEUP_STATUS);
#ifdef FPGA_BOARD_ARTINCHIP
    #define CMU_SC_CLK_CPU              0x980201f0
    #define CMU_CSYS_CLK_CPU            0x98020200
    //reset SC CPU
    writel(0xac000000, CMU_SC_CLK_CPU);
    //reset CSYS CPU, release by non-secure se brom
    writel(0xac000000, CMU_CSYS_CLK_CPU);
#endif
    aicos_irq_enable(GMAC0_IRQn);
    aicos_irq_enable(RTC_IRQn);
    /* reset all pins */
    //TO DO
#ifndef AIC_USING_SRAM
    aic_ddr_sr_code_on_ddr();
#else
    aic_ddr_sr_code_on_sram();
#endif

    /* wakeup flow */
    aicos_irq_disable(GMAC0_IRQn);
    aicos_irq_disable(RTC_IRQn);
    /* restore CMU pll and bus freqency */
    for (i = 0; i < ARRAY_SIZE(cmu_pll_freq); i++)
        hal_clk_set_freq(CLK_CS_PLL_FRA0 + i, cmu_pll_freq[i]);

    writel(cmu_apb0_freq, CMU_APB0_REG);
    writel(cmu_apb2_freq, CMU_APB2_REG);

    /* indicate DDR has exited self-refresh and is ready */
    reg_val = readl(PRCM_DDR_WAKEUP_STATUS);
    reg_val &= ~1;
    writel(reg_val, PRCM_DDR_WAKEUP_STATUS);
    /* restore the interrupt status of each peripheral  */
    for (i = 0; i < MAX_IRQn; i++)
    {
        if (save_sp_clic_ie[i])
            aicos_irq_enable(i);
    }
    //release SC CPU, CSYS CPU is released by se brom
    writel(0xac003100, CMU_SC_CLK_CPU);
    /* change cpu frequency to pll */
    hal_clk_set_parent(CLK_CPU, CLK_PLL_FRA0);
    /* enable PLL_INT1: bus pll */
    hal_clk_set_parent(CLK_APB0, CLK_PLL_FRA0);
    hal_clk_set_parent(CLK_AHB, CLK_PLL_FRA0);
    rt_pm_request(PM_SLEEP_MODE_NONE);
    rt_hw_interrupt_enable(level);
}


static void aic_sleep(struct rt_pm *pm, uint8_t mode)
{
    switch (mode)
    {
    case PM_SLEEP_MODE_NONE:
        break;

    case PM_SLEEP_MODE_IDLE:
        aic_pm_enter_idle();
        break;
    case PM_SLEEP_MODE_LIGHT:
        break;
    case PM_SLEEP_MODE_DEEP:
        aic_pm_enter_deep_sleep();
        break;
    case PM_SLEEP_MODE_STANDBY:
        //TO DO
        break;
    case PM_SLEEP_MODE_SHUTDOWN:
        break;
    default:
        RT_ASSERT(0);
        break;
    }
}

/* timeout unit is rt_tick_t, but MTIMECMPH/L unit is HZ
 * one tick is 4000 counter
 */
static void aic_timer_start(struct rt_pm *pm, rt_uint32_t timeout)
{
    uint64_t tmp_counter;
    uint32_t tick_resolution = drv_get_sys_freq() / CONFIG_SYSTICK_HZ;

    sleep_counter = ((uint64_t)csi_coret_get_valueh() << 32) |
                    csi_coret_get_value();
    tmp_counter = (uint64_t)timeout * tick_resolution;

    csi_coret_set_load(tmp_counter + sleep_counter);
}

static void aic_timer_stop(struct rt_pm *pm)
{
    uint64_t tmp_counter = ((uint64_t)csi_coret_get_valueh() << 32) |
                           csi_coret_get_value();

    uint32_t tick_resolution = drv_get_sys_freq() / CONFIG_SYSTICK_HZ;

    csi_coret_set_load(tmp_counter + tick_resolution);
}

static rt_tick_t aic_timer_get_tick(struct rt_pm *pm)
{
    rt_tick_t delta_tick;
    uint64_t delta_counter;
    uint32_t tick_resolution = drv_get_sys_freq() / CONFIG_SYSTICK_HZ;

    resume_counter = ((uint64_t)csi_coret_get_valueh() << 32) |
                     csi_coret_get_value();
    delta_counter = resume_counter - sleep_counter;

    delta_tick = delta_counter / tick_resolution;

    return delta_tick;
}

static const struct rt_pm_ops aic_pm_ops =
{
    aic_sleep,
    NULL,
    aic_timer_start,
    aic_timer_stop,
    aic_timer_get_tick,
};

/**
 * This function initialize the power manager
 */

int aic_pm_hw_init(void)
{
    rt_uint8_t timer_mask = 0;

    timer_mask = 1UL << PM_SLEEP_MODE_DEEP;
    /* initialize system pm module */
    rt_system_pm_init(&aic_pm_ops, timer_mask, RT_NULL);

    return 0;
}
INIT_BOARD_EXPORT(aic_pm_hw_init);
