/*
 * Copyright (C) 2017-2024 Alibaba Group Holding Limited
 */

#include <stdint.h>
#include <soc.h>
#include <sys_clk.h>
#include <drv/io.h>

uint32_t g_system_clock = IHS_VALUE;

#if CONFIG_BOARD_XIAOHUI_EVB
uint32_t soc_get_cpu_freq(uint32_t idx)
{
    return 50*1000000;
}

uint32_t soc_get_coretim_freq(void)
{
    return 25*1000000;
}

uint32_t soc_get_uart_freq(uint32_t idx)
{
    return 36*1000000;
}

uint32_t soc_get_timer_freq(uint32_t idx)
{
    return 25*1000000;
}

#else
uint32_t soc_get_cpu_freq(uint32_t idx)
{
    return g_system_clock;
}

uint32_t soc_get_cur_cpu_freq(void)
{
    return g_system_clock;
}

uint32_t soc_get_coretim_freq(void)
{
    return g_system_clock;
}

uint32_t soc_get_uart_freq(uint32_t idx)
{
    return g_system_clock;
}

csi_error_t soc_sysclk_config(system_clk_config_t *config)
{
    return CSI_OK;
}

void soc_reset_uart(uint32_t idx)
{
}

uint32_t soc_get_timer_freq(uint32_t idx)
{
    return g_system_clock;
}
#endif

#warning "fix me"

struct aic_sysclk
{
    unsigned long       freq;
    unsigned int        clk_id;
    unsigned int        parent_clk_id;
};

#define AIC_CLK_PLL_INT0_FREQ 480000000
#define AIC_CLK_PLL_INT1_FREQ 1200000000
#define AIC_CLK_PLL_FRA0_FREQ 792000000
#define AIC_CLK_PLL_FRA2_FREQ 1188000000
#define AIC_CLK_CPU_FREQ 480000000
#define AIC_CLK_AXI0_FREQ 200000000
#define AIC_CLK_AHB0_FREQ 200000000
#define AIC_CLK_APB0_FREQ 100000000

#include "aic_clk_id.h"

struct aic_sysclk aic_sysclk_config[] = {
    {AIC_CLK_PLL_INT0_FREQ, CLK_PLL_INT0, 0},           /* 480000000 */
    {AIC_CLK_PLL_INT1_FREQ, CLK_PLL_INT1, 0},           /* 1200000000 */
    {AIC_CLK_PLL_FRA0_FREQ, CLK_PLL_FRA0, 0},           /* 792000000 */
    {AIC_CLK_PLL_FRA2_FREQ, CLK_PLL_FRA2, 0},           /* 1188000000 */
    {AIC_CLK_CPU_FREQ, CLK_CPU, CLK_CPU_SRC1},          /* 480000000 */
    {AIC_CLK_AXI0_FREQ, CLK_AXI0, CLK_AXI_AHB_SRC1},    /* 200000000 */
    {AIC_CLK_AHB0_FREQ, CLK_AHB0, CLK_AXI_AHB_SRC1},    /* 200000000 */
    {AIC_CLK_APB0_FREQ, CLK_APB0, CLK_APB0_SRC1},       /* 100000000 */
//    {24000000, CLK_APB1, 0},
   {25000000, CLK_OUT2, 0},
};

/*
 * Some Chips may enable USB0 EHCI in Boot ROM,
 * it is better to disable USB0 EHCI during boot to avoid some side effect.
 */
static void usb_ehci_disable(void)
{
    hal_clk_disable_assertrst(CLK_USBH0);
    hal_clk_disable(CLK_USBH0);
}

void aic_board_sysclk_init(void)
{
    uint32_t i = 0;

    usb_ehci_disable();

    for (i=0; i<sizeof(aic_sysclk_config)/sizeof(struct aic_sysclk); i++) {
        if (aic_sysclk_config[i].freq == 0)
            continue;

        /* multi parent clk */
        if (aic_sysclk_config[i].parent_clk_id) {
            hal_clk_set_freq(aic_sysclk_config[i].parent_clk_id,
                             aic_sysclk_config[i].freq);
            hal_clk_enable(aic_sysclk_config[i].parent_clk_id);
            hal_clk_set_parent(aic_sysclk_config[i].clk_id,
                               aic_sysclk_config[i].parent_clk_id);
        } else {
            hal_clk_set_freq(aic_sysclk_config[i].clk_id, aic_sysclk_config[i].freq);
            hal_clk_enable(aic_sysclk_config[i].clk_id);
        }
    }

    /* Enable sys clk */
    hal_clk_enable_deassertrst_iter(CLK_GPIO);
    hal_clk_enable_deassertrst_iter(CLK_GTC);
#ifdef AIC_USING_GMAC0
    hal_clk_enable_iter(CLK_OUT2);
#endif
}





void soc_clk_enable(int32_t module)
{
}

void soc_clk_disable(int32_t module)
{
}

void soc_set_sys_freq(uint32_t val)
{
    g_system_clock = val;
}

int32_t drv_get_sys_freq(void)
{
    return g_system_clock;
}


