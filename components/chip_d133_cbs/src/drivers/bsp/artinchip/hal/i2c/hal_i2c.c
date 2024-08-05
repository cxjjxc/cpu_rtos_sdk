/*
 * Copyright (c) 2022-2023, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: geo <guojun.dong@artinchip.com>
 */
#include <rtconfig.h>
#include <stdbool.h>
#include <string.h>
#include <hal_i2c.h>
#include "aic_errno.h"

#define gen_reg(val) (volatile void *)(val)
#define USEC_PER_SEC (1000000)

int hal_i2c_clk_init(aic_i2c_ctrl *i2c_dev)
{
    int ret = 0;
    ret = hal_clk_enable_deassertrst(CLK_I2C0 + i2c_dev->index);
    if (ret < 0)
        pr_err("I2C clock and reset init error\n");
    return ret;
}

void hal_i2c_set_hold(aic_i2c_ctrl *i2c_dev, u32 val)
{
    writel(val, i2c_dev->reg_base + I2C_SDA_HOLD);
}

int hal_i2c_set_master_slave_mode(aic_i2c_ctrl *i2c_dev)
{
    uint32_t reg_val;
    uint8_t mode;

    CHECK_PARAM(i2c_dev, -EINVAL);

    mode = i2c_dev->bus_mode;
    reg_val = readl(gen_reg(i2c_dev->reg_base + I2C_CTL));
    reg_val &= ~I2C_CTL_MASTER_SLAVE_SELECT_MASK;
    if (!mode)
        reg_val |= I2C_ENABLE_MASTER_DISABLE_SLAVE;
    else
        /* slave mode, and will detect stop signal only if addressed */
        reg_val |= I2C_CTL_STOP_DET_IFADDR;

    writel(reg_val, gen_reg(i2c_dev->reg_base + I2C_CTL));

    return 0;
}

int hal_i2c_master_10bit_addr(aic_i2c_ctrl *i2c_dev)
{
    uint32_t reg_val;
    uint8_t enable;

    CHECK_PARAM(i2c_dev, -EINVAL);

    enable = i2c_dev->addr_bit;
    reg_val = readl(gen_reg(i2c_dev->reg_base + I2C_CTL));
    reg_val &= ~I2C_CTL_10BIT_SELECT_MASTER;
    if (enable)
        reg_val |= I2C_CTL_10BIT_SELECT_MASTER;

    writel(reg_val, gen_reg(i2c_dev->reg_base + I2C_CTL));

    return 0;
}

int hal_i2c_slave_10bit_addr(aic_i2c_ctrl *i2c_dev)
{
    uint32_t reg_val;
    uint8_t enable;
    CHECK_PARAM(i2c_dev, -EINVAL);

    enable = i2c_dev->addr_bit;
    reg_val = readl(gen_reg(i2c_dev->reg_base + I2C_CTL));
    reg_val &= ~I2C_CTL_10BIT_SELECT_SLAVE;
    if (enable)
        reg_val |= I2C_CTL_10BIT_SELECT_SLAVE;

    writel(reg_val, gen_reg(i2c_dev->reg_base + I2C_CTL));

    return 0;
}

static int hal_i2c_scl_cnt(uint32_t clk_freq, uint8_t is_standard_speed,
                       uint16_t *hcnt, uint16_t *lcnt)
{
    uint16_t hcnt_tmp, lcnt_tmp;

    CHECK_PARAM(hcnt, -EINVAL);
    CHECK_PARAM(lcnt, -EINVAL);

    if (is_standard_speed) {
        /* Minimum value of tHIGH in standard mode is 4000ns
                 * Plus 2 is just to increase the time of tHIGH, appropriately.
                 * SS_MIN_SCL_HIGH * (clk_freq / 1000) is just to prevent 32bits
                 * overflow. SS_MIN_SCL_HIGH * clk_freq will 32bits overflow.
                 */
        hcnt_tmp = SS_MIN_SCL_HIGH * (clk_freq / 1000) / 1000000 + 2;
        lcnt_tmp = SS_MIN_SCL_LOW * (clk_freq / 1000) / 1000000 + 2;
    } else {
        /* If isStandardSpeed is false, set i2c to fast speed
                 * Minimum value of tHIGH in fast mode is 600ns
                 * Plus 3 is just to increase the time of tHIGH, appropriately.
                 * FS_MIN_SCL_HIGH * (clk_freq / 1000)
                 */
        hcnt_tmp = FS_MIN_SCL_HIGH * (clk_freq / 1000) / 1000000 + 2;
        lcnt_tmp = FS_MIN_SCL_LOW * (clk_freq / 1000) / 1000000 + 2;
    }

    *hcnt = hcnt_tmp;
    *lcnt = lcnt_tmp;

    return 0;
}

int hal_i2c_speed_mode_select(aic_i2c_ctrl *i2c_dev, uint32_t clk_freq, uint8_t mode)
{
    uint32_t reg_val;
    uint16_t hcnt, lcnt;
    int ret;

    CHECK_PARAM(i2c_dev, -EINVAL);

    reg_val = readl(gen_reg(i2c_dev->reg_base + I2C_CTL));
    reg_val &= ~I2C_CTL_SPEED_MODE_SELECT_MASK;
    if (!mode) {
        reg_val |= I2C_CTL_SPEED_MODE_FS;
        /* Calculate fast speed HCNT and LCNT */
        ret = hal_i2c_scl_cnt(clk_freq, false, &hcnt, &lcnt);
        if (ret)
            return ret;

        writel(hcnt, gen_reg(i2c_dev->reg_base + I2C_FS_SCL_HCNT));
        writel(lcnt, gen_reg(i2c_dev->reg_base + I2C_FS_SCL_LCNT));
    } else {
        reg_val |= I2C_CTL_SPEED_MODE_SS;
        /* Calculate standard speed HCNT and LCNT */
        ret = hal_i2c_scl_cnt(clk_freq, true, &hcnt, &lcnt);
        if (ret)
            return ret;

        writel(hcnt, gen_reg(i2c_dev->reg_base + I2C_SS_SCL_HCNT));
        writel(lcnt, gen_reg(i2c_dev->reg_base + I2C_SS_SCL_LCNT));
    }

    writel(reg_val, gen_reg(i2c_dev->reg_base + I2C_CTL));

    return 0;
}

void hal_i2c_target_addr(aic_i2c_ctrl *i2c_dev, uint32_t addr)
{
    uint32_t reg_val;

    reg_val = readl(gen_reg(i2c_dev->reg_base + I2C_TAR));
    reg_val &= ~I2C_TAR_ADDR_MASK;
    reg_val |= addr;

    writel(reg_val, gen_reg(i2c_dev->reg_base + I2C_TAR));
}

int hal_i2c_slave_own_addr(aic_i2c_ctrl *i2c_dev, uint32_t addr)
{
    CHECK_PARAM(i2c_dev, -EINVAL);
    CHECK_PARAM(!(addr > I2C_TAR_ADDR_MASK), -EINVAL);

    writel(addr, gen_reg(i2c_dev->reg_base + I2C_SAR));

    return 0;
}

int hal_i2c_init(aic_i2c_ctrl *i2c_dev)
{
    int32_t ret = I2C_OK;

    ret = hal_i2c_clk_init(i2c_dev);
    if (ret)
        return ret;

    hal_i2c_set_master_slave_mode(i2c_dev);
    hal_i2c_set_hold(i2c_dev, 10);
    hal_i2c_master_10bit_addr(i2c_dev);
    hal_i2c_slave_10bit_addr(i2c_dev);
#ifdef AIC_I2C_INTERRUPT_MODE
    hal_i2c_disable_all_irq(i2c_dev);
    hal_i2c_set_transmit_fifo_threshold(i2c_dev);
#else
    hal_i2c_master_enable_irq(i2c_dev);
#endif
    ret = hal_i2c_speed_mode_select(i2c_dev, I2C_DEFALT_CLOCK, i2c_dev->speed_mode);
    if (ret)
        return ret;

    if (i2c_dev->bus_mode) {
        hal_i2c_config_fifo_slave(i2c_dev);
        hal_i2c_clear_all_irq_flags(i2c_dev);
        hal_i2c_slave_enable_irq(i2c_dev);
        hal_i2c_module_disable(i2c_dev);
        hal_i2c_slave_own_addr(i2c_dev, i2c_dev->slave_addr);
        hal_i2c_module_enable(i2c_dev);
    }

    return ret;
}

static int32_t hal_i2c_wait_transmit(aic_i2c_ctrl *i2c_dev, uint32_t timeout)
{
    int32_t ret = I2C_OK;

    do {
        uint64_t timecount = timeout + aic_get_time_ms();

        while ((hal_i2c_get_transmit_fifo_num(i2c_dev) != 0U) &&
               (ret == EOK)) {
            if (aic_get_time_ms() >= timecount) {
                ret = I2C_TIMEOUT;
            }
        }

    } while (0);

    return ret;
}

static int32_t hal_i2c_wait_receive(aic_i2c_ctrl *i2c_dev,
                                    uint32_t wait_data_num, uint32_t timeout)
{
    int32_t ret = I2C_OK;

    do {
        uint64_t timecount = timeout + aic_get_time_ms();

        while ((hal_i2c_get_receive_fifo_num(i2c_dev) < wait_data_num) &&
               (ret == I2C_OK)) {
            if (aic_get_time_ms() >= timecount) {
                ret = I2C_TIMEOUT;
            }
        }
    } while (0);

    return ret;
}

int32_t hal_i2c_wait_bus_free(aic_i2c_ctrl *i2c_dev, uint32_t timeout)
{
    int32_t ret = I2C_OK;

    uint64_t timecount = timeout + aic_get_time_ms();

    while (hal_i2c_bus_status(i2c_dev)) {
        if (aic_get_time_ms() >= timecount) {
            ret = I2C_TIMEOUT;
            return ret;
        }
    }

    return ret;
}

/**
  \brief       hal_i2c_master_send_msg
  \param[in]   reg_base
  \param[in]
  \return      bytes of sent msg
*/
int32_t hal_i2c_master_send_msg(aic_i2c_ctrl *i2c_dev,
                                struct aic_i2c_msg *msg, uint8_t is_last_message)
{
    CHECK_PARAM(msg, -EINVAL);

    int32_t ret = I2C_OK;
    uint16_t size = msg->len;
    uint32_t send_count = 0;
    uint32_t stop_time = 0;
    uint32_t timeout = 10;
    uint32_t reg_val;
    uint16_t idx = 0;

    hal_i2c_module_disable(i2c_dev);
    hal_i2c_target_addr(i2c_dev, msg->addr);
    hal_i2c_module_enable(i2c_dev);

    if (!size)
    {
        hal_i2c_transmit_data_with_stop_bit(i2c_dev, 0);
        while (1)
        {
            reg_val = readl(i2c_dev->reg_base + I2C_INTR_RAW_STAT);
            if (reg_val & I2C_INTR_STOP_DET)
            {
                if (reg_val & I2C_INTR_TX_ABRT)
                {
                    return -1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }

    while (size > 0) {
        uint16_t send_num = size > I2C_FIFO_DEPTH ? I2C_FIFO_DEPTH : size;
        for (uint16_t i = 0; i < send_num; i++) {
            if (is_last_message && (idx == msg->len -1))
                hal_i2c_transmit_data_with_stop_bit(i2c_dev, msg->buf[idx]);
            else if (!is_last_message && (idx == 0))
                hal_i2c_set_restart_bit_with_data(i2c_dev, msg->buf[idx]);
            else
                hal_i2c_transmit_data(i2c_dev, msg->buf[idx]);
            idx++;
        }
        size -= send_num;
        send_count += send_num;

        ret = hal_i2c_wait_transmit(i2c_dev, timeout);
        if (ret != I2C_OK) {
            send_count = ret;
            return I2C_TIMEOUT;
        }
    }

    while (!(hal_i2c_get_raw_interrupt_state(i2c_dev)
            & (I2C_INTR_STOP_DET | I2C_INTR_START_DET))) {
        stop_time++;
        if (stop_time > I2C_TIMEOUT_DEF_VAL) {
            return I2C_TIMEOUT;
        }
    }

    return send_count;
}

/**
  \brief       hal_i2c_master_receive_msg
  \param[in]   reg_base
  \param[in]
  \return      bytes of read msg
*/
int32_t hal_i2c_master_receive_msg(aic_i2c_ctrl *i2c_dev,
                                   struct aic_i2c_msg *msg, uint8_t is_last_message)
{
    CHECK_PARAM(msg, -EINVAL);

    int32_t ret = I2C_OK;
    uint16_t size = msg->len;
    uint32_t read_count = 0;
    uint8_t *receive_data = msg->buf;
    uint32_t timeout = 10;
    int idx = 0, count = 0;
    CHECK_PARAM(receive_data, -EINVAL);

    hal_i2c_module_disable(i2c_dev);
    hal_i2c_target_addr(i2c_dev, msg->addr);
    hal_i2c_module_enable(i2c_dev);

    while (size > 0) {
        int32_t recv_num = size > I2C_FIFO_DEPTH ? I2C_FIFO_DEPTH : size;
        for (uint16_t len = 0; len < recv_num; len++) {
            if (is_last_message && (count == msg->len - 1))
                hal_i2c_read_data_cmd_with_stop_bit(i2c_dev);
            else
                hal_i2c_read_data_cmd(i2c_dev);
            count++;
        }

        size -= recv_num;
        read_count += recv_num;
        ret = hal_i2c_wait_receive(i2c_dev, recv_num, timeout);
        if (ret == I2C_OK) {
            for (uint16_t i = 0; i < recv_num; i++) {
                receive_data[idx] = hal_i2c_get_receive_data(i2c_dev);
                idx++;
            }
        } else {
            read_count = (int32_t)ret;
            break;
        }
    }

    uint32_t timecount = timeout + aic_get_time_ms();

    while (!(hal_i2c_get_raw_interrupt_state(i2c_dev)
            & (I2C_INTR_STOP_DET | I2C_INTR_START_DET))) {
        if (aic_get_time_ms() >= timecount) {
            return I2C_TIMEOUT;
            break;
        }
    }
    return read_count;
}

