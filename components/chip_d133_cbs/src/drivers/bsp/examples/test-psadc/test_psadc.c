/*
 * Copyright (c) 2024, Artinchip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: Li Siyao <siyao.li@artinchip.com>
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <console.h>
#include <getopt.h>

#include "hal_psadc.h"
#include "test_psadc.h"
#include "aic_hal_clk.h"

/* The default voltages are set to D21x->3.0V, D13x、D12x->2.5V */
#define AIC_PSADC_STANDARD_VOLTAGE       3
#define AIC_PSADC_ADC_MAX_VAL            0xFFF
#define AIC_PSADC_VOLTAGE_ACCURACY       10000
#define AIC_PSADC_DEFAULT_SAMPLES_NUM    10

static float g_def_voltage = AIC_PSADC_STANDARD_VOLTAGE;
static int g_sample_num = AIC_PSADC_DEFAULT_SAMPLES_NUM;
static struct aic_psadc_queue *queue = &aic_psadc_queues[AIC_PSADC_QC];

/* Functions */

static void cmd_psadc_usage(void)
{
    printf("Compile time: %s %s\n", __DATE__, __TIME__);
    printf("Usage: test_psadc [options]\n");
    printf("test_psadc read                       : Get the adc value\n");
    printf("test_psadc modify <standard_voltage>  : Modify standard voltage, default is 3V\n");
    printf("test_psadc status                     : Check the psadc status\n");
    printf("test_psadc help                       : Get this help\n");
    printf("\n");
    printf("Example: test_psadc read\n");
}

static void adc2voltage(float st_voltage, int adc_value)
{
    int voltage;

    voltage = (adc_value * st_voltage * 100) / AIC_PSADC_ADC_MAX_VAL;
    printf(" %d.%2dv", voltage / 100, voltage % 100);
    return;
}

static int test_psadc_init()
{
    int cnt = 0;
    int ret;

#ifdef AIC_PSADC_DRV_V11
    ret = hal_clk_set_freq(CLK_PSADC, AIC_PSADC_CLK_RATE);
    if (ret < 0) {
            printf("PSADC clk freq set failed!");
            return -EINVAL;
    }
#endif

    ret = hal_clk_enable(CLK_PSADC);
    if (ret < 0) {
        printf("PSADC clk enable failed!");
        return -EINVAL;
    }

    ret = hal_clk_enable_deassertrst(CLK_PSADC);
    if (ret < 0) {
        printf("PSADC reset deassert failed!");
        return -EINVAL;
    }

    hal_psadc_single_queue_mode(1);
    hal_psadc_enable(1);
    hal_psadc_set_ch_num(ARRAY_SIZE(aic_psadc_chs));

    for (int i = 0; i < AIC_PSADC_CH_NUM; i++) {
        struct aic_psadc_ch *chan = hal_psadc_ch_is_valid(i);
        if (!chan)
            continue;
        if (chan->available && cnt < AIC_PSADC_QUEUE_LENGTH) {
            hal_psadc_set_queue_node(AIC_PSADC_Q1, chan->id, cnt);
            cnt++;
            continue;
        }
        if (chan->available && cnt >= AIC_PSADC_QUEUE_LENGTH) {
            hal_psadc_set_queue_node(AIC_PSADC_Q2, chan->id,
                                    cnt - AIC_PSADC_QUEUE_LENGTH);
            cnt++;
            continue;
        }
    }
    queue->nodes_num = cnt;
    queue->complete = aicos_sem_create(0);
    hal_psadc_ch_init();

    return 0;
}

static void cmd_psadc_set(int argc, char **argv)
{
    g_def_voltage = strtod(argv[1], NULL);
    if (g_def_voltage < 0) {
        printf("Please input valid default voltage\n");
        return;
    }
    printf("Successfully set the default voltage\n");

    return;
}

static int test_psadc_get_adc()
{
    int ret = 0;
    u32 adc_values[AIC_PSADC_CH_NUM];
    int cnt = 0;
    int chan_cnt = 0;
    u64 start_us, end_us;

    ret = test_psadc_init();
    if (ret < 0) {
        printf("PSADC init failed\n");
        return -EINVAL;
    }

    chan_cnt = queue->nodes_num;
    if (!chan_cnt) {
        printf("Please enable the required channel in menuconfig");
        return -EINVAL;
    }

    printf("Starting sampling for %d channels\n", chan_cnt);

    while (cnt < g_sample_num) {
        cnt++;

        start_us = aic_get_time_us();
        hal_psadc_read_poll(adc_values, AIC_PSADC_POLL_READ_TIMEOUT);
        end_us = aic_get_time_us();
        printf("Sample time: %d us\n", abs(end_us - start_us));
        if (ret < 0) {
            printf("Read timeout!\n");
        }

        printf("[%d] PSADC: ", cnt);
        for (int i = 0; i < chan_cnt; i++) {
            printf(" %d", adc_values[i]);
        }
        printf("\nvoltage: ");
        for (int i = 0; i < chan_cnt; i++) {
            adc2voltage(g_def_voltage, adc_values[i]);
        }
        printf("\n");

    }

    hal_psadc_qc_irq_enable(0);
    aicos_sem_delete(queue->complete);
    queue->complete = NULL;

    return 0;
}

static int cmd_test_psadc(int argc, char *argv[])
{
    if (argc < 2) {
        cmd_psadc_usage();
        return 0;
    }

    if (!strcmp(argv[1], "read")) {
        test_psadc_get_adc();
        return 0;
    }

    if (!strcmp(argv[1], "status")) {
        aich_psadc_status_show();
        return 0;
    }

    if (!strcmp(argv[1], "modify")) {
        cmd_psadc_set(argc - 1, &argv[1]);
        return 0;
    }

    if (!strcmp(argv[1], "help")) {
        cmd_psadc_usage();
        return 0;
    }

    return 0;
}

CONSOLE_CMD(test_psadc, cmd_test_psadc,  "PSADC test example");
