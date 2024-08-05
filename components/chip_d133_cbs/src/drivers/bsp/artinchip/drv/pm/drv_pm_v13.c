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

extern void sc_save_context_and_suspend();
extern void sc_restore_context_and_resume();
extern u32 sc_restore_context_and_resume_size;

void aic_pm_enter_idle(void)
{
    __WFI();
}

void aic_pm_enter_deep_sleep(void)
{
    uint32_t i;
    uint8_t save_sc_clic_ie[MAX_IRQn] = {0};
    uint32_t save_sc_context[36] = {0};

    rt_memcpy((void *)0x80050000, sc_restore_context_and_resume,
                sc_restore_context_and_resume_size);

    /* save the interrupt status of each peripheral  */
    for (i = 0; i < MAX_IRQn; i++)
    {
        save_sc_clic_ie[i] = (uint8_t)csi_vic_get_enabled_irq(i);
        if (save_sc_clic_ie[i])
            aicos_irq_disable(i);
    }

    /* save context and wait power down */
    sc_save_context_and_suspend(&save_sc_context);

    /* SC power up and resume flow */
    CLIC->CLICCFG = (((CLIC->CLICINFO & CLIC_INFO_CLICINTCTLBITS_Msk) >>
                      CLIC_INFO_CLICINTCTLBITS_Pos) << CLIC_CLICCFG_NLBIT_Pos);
    /* config CLIC attribute to use vector interrupt */
    for (i = 0; i < MAX_IRQn; i++)
        CLIC->CLICINT[i].ATTR = 1;

    CLIC->CLICINT[Machine_Software_IRQn].ATTR = 0x3;

    /* restore the interrupt status of each peripheral  */
    for (i = 0; i < MAX_IRQn; i++)
    {
        if (save_sc_clic_ie[i])
            aicos_irq_enable(i);
    }

    rt_pm_request(PM_SLEEP_MODE_NONE);
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
