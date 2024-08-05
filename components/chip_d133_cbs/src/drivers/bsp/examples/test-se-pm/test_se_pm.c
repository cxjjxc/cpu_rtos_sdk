/*
 * Copyright (c) 2023-2024, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: dwj <weijie.ding@artinchip.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <console.h>
#include <aic_errno.h>
#include <aic_core.h>
#include <aic_common.h>
#include <aic_hal.h>
#include <hal_rtc.h>
#include "hal_wdt.h"
#include <wdt.h>

extern void aic_se_suspend_and_resume(void);

static int test_se_pm(int argc, char *argv[])
{
    int i = 0;

    aic_se_suspend_and_resume();

    for (i = 0; i < 5; i++)
    {
        printf("i = %d\n", i);
    }

    return 0;
}

CONSOLE_CMD(test_pm, test_se_pm, "SE suspend/resume test case");

static int test_se_pm_reset(int argc, char *argv[])
{
    int timeout;
    wdt_init();

    timeout = strtoul(argv[1], NULL, 10);

    //Only reset SE CPU
    hal_wdt_rst_type_set(1);

    wdt_start(timeout);

    return 0;
}
CONSOLE_CMD(se_reset, test_se_pm_reset, "reset se cpu by watchdog");
