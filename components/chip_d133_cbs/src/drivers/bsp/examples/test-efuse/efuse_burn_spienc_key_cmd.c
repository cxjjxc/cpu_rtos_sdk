/*
 * Copyright (c) 2022-2023, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: Xiong Hao <hao.xiong@artinchip.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <efuse.h>
#include <console.h>
#include <aic_utils.h>
#include "spi_aes_key.h"

// #define D12X_BURN_SPIENC_KEY_ENABLE

/* The eFuse size */
#define D12X_EFUSE_SIZE (512 / 8)

int burn_brom_spienc_bit(void)
{
    u32 offset, val;
    int ret;

    offset = 4;
    val = 0;
    val |= (1 << 28); // SPIENC boot bit for brom
    ret = efuse_program(offset, (const void *)&val, 4);
    if (ret <= 0) {
        printf("Write BROM SPIENC bit error\n");
        return -1;
    }

    offset = D12X_EFUSE_SIZE + 4;
    ret = efuse_program(offset, (const void *)&val, 4);
    if (ret <= 0) {
        printf("Write BROM SPIENC bit error (backup area)\n");
        return -1;
    }

    return 0;
}

int check_brom_spienc_bit(void)
{
    u32 offset, val;
    int ret;

    offset = 4;
    ret = efuse_read(offset, (void *)&val, 4);
    if (ret <= 0) {
        printf("Read secure bit efuse error.\n");
        return -1;
    }
    if (val & (1 << 28)) {
        printf("BROM SPIENC is ENABLED\n");
    } else {
        printf("BROM SPIENC is NOT enabled\n");
    }

    offset = D12X_EFUSE_SIZE + 4;
    val = 0;
    ret = efuse_read(offset, (void *)&val, 4);
    if (ret <= 0) {
        printf("Read secure bit efuse error.\n");
        return -1;
    }
    if (val & (1 << 28)) {
        printf("BROM SPIENC is ENABLED (backup area)\n");
    } else {
        printf("BROM SPIENC is NOT enabled (backup area)\n");
    }

    return 0;
}

int burn_jtag_lock_bit(void)
{
    u32 offset, val;
    int ret;

    offset = 4;
    val = 0;
    val |= (1 << 24); // JTAG LOCK
    ret = efuse_program(offset, (const void *)&val, 4);
    if (ret <= 0) {
        printf("Write JTAG LOCK bit error\n");
        return -1;
    }

    offset = D12X_EFUSE_SIZE + 4;
    ret = efuse_program(offset, (const void *)&val, 4);
    if (ret <= 0) {
        printf("Write JTAG LOCK bit error (backup area)\n");
        return -1;
    }

    return 0;
}

int check_jtag_lock_bit(void)
{
    u32 offset, val;
    int ret;

    offset = 4;
    ret = efuse_read(offset, (void *)&val, 4);
    if (ret <= 0) {
        printf("Read secure bit efuse error.\n");
        return -1;
    }
    if (val & (1 << 24)) {
        printf("JTAG LOCK   is ENABLED\n");
    } else {
        printf("JTAG LOCK   is NOT enabled\n");
    }

    offset = D12X_EFUSE_SIZE + 4;
    val = 0;
    ret = efuse_read(offset, (void *)&val, 4);
    if (ret <= 0) {
        printf("Read secure bit efuse error.\n");
        return -1;
    }
    if (val & (1 << 28)) {
        printf("JTAG LOCK   is ENABLED (backup area)\n");
    } else {
        printf("JTAG LOCK   is NOT enabled (backup area)\n");
    }

    return 0;
}

int burn_spienc_key(void)
{
    u32 offset;
    int ret;

    offset = 0x20;
    ret = efuse_program(offset, (const void *)spi_aes_key, spi_aes_key_len);
    if (ret <= 0) {
        printf("Write SPI ENC AES key error.\n");
        return -1;
    }

    offset = D12X_EFUSE_SIZE + 0x20;
    ret = efuse_program(offset, (const void *)spi_aes_key, spi_aes_key_len);
    if (ret <= 0) {
        printf("Write SPI ENC AES key error (backup area).\n");
        return -1;
    }

    return 0;
}

int check_spienc_key(void)
{
    u32 offset;
    u8 data[256];
    int ret;

    offset = 0x20;
    ret = efuse_read(offset, (void *)data, 16);
    if (ret <= 0) {
        printf("Read efuse error.\n");
        return -1;
    }
    printf("SPI ENC KEY:\n");
    hexdump(data, 16, 1);

    offset = D12X_EFUSE_SIZE + 0x20;
    ret = efuse_read(offset, (void *)data, 16);
    if (ret <= 0) {
        printf("Read efuse error.\n");
        return -1;
    }
    printf("SPI ENC KEY(bakcup area):\n");
    hexdump(data, 16, 1);

    return 0;
}

int burn_spienc_key_read_write_disable_bits(void)
{
    u32 offset, val;
    int ret;

    offset = 0;
    val = 0;
    val = 0x0F000F00; // SPIENC Key Read/Write disable
    ret = efuse_program(offset, (const void *)&val, 4);
    if (ret <= 0) {
        printf("Write r/w disable bit efuse error.\n");
        return -1;
    }

    offset = D12X_EFUSE_SIZE + 0;
    ret = efuse_program(offset, (const void *)&val, 4);
    if (ret <= 0) {
        printf("Write r/w disable bit efuse error (backup area).\n");
        return -1;
    }

    return 0;
}


int check_spienc_key_read_write_disable_bits(void)
{
    u32 offset, val;
    int ret;

    offset = 0;
    ret = efuse_read(offset, (void *)&val, 4);
    if (ret <= 0) {
        printf("Read r/w disable bit efuse error.\n");
        return -1;
    }

    if ((val & 0xF00) == 0xF00)
        printf("SPI ENC Key is read DISABLED\n");
    else
        printf("SPI ENC Key is NOT read disabled\n");
    if (((val>>16) & 0xF00) == 0xF00)
        printf("SPI ENC Key is write DISABLED\n");
    else
        printf("SPI ENC Key is NOT write disabled\n");

    offset = D12X_EFUSE_SIZE + 0;
    val = 0;
    ret = efuse_read(offset, (void *)&val, 4);
    if (ret <= 0) {
        printf("Read r/w disable bit efuse error.\n");
        return -1;
    }

    if ((val & 0xF00) == 0xF00)
        printf("SPI ENC Key is read  DISABLED (backup area)\n");
    else
        printf("SPI ENC Key is NOT read  disabled (backup area)\n");
    if (((val>>16) & 0xF00) == 0xF00)
        printf("SPI ENC Key is write DISABLED (backup area)\n");
    else
        printf("SPI ENC Key is NOT write disabled (backup area)\n");

    return 0;
}


int cmd_efuse_do_d12x_spienc(int argc, char **argv)
{
    int ret;

    efuse_init();

#ifdef D12X_BURN_SPIENC_KEY_ENABLE
    ret = burn_brom_spienc_bit();
    if (ret) {
        printf("Error\n");
        return -1;
    }

    ret = burn_spienc_key();
    if (ret) {
        printf("Error\n");
        return -1;
    }

    ret = burn_spienc_key_read_write_disable_bits();
    if (ret) {
        printf("Error\n");
        return -1;
    }

    ret = burn_jtag_lock_bit();
    if (ret) {
        printf("Error\n");
        return -1;
    }
#endif

    ret = check_brom_spienc_bit();
    if (ret) {
        printf("Error\n");
        return -1;
    }

    ret = check_jtag_lock_bit();
    if (ret) {
        printf("Error\n");
        return -1;
    }
    ret = check_spienc_key();
    if (ret) {
        printf("Error\n");
        return -1;
    }

    ret = check_spienc_key_read_write_disable_bits();
    if (ret) {
        printf("Error\n");
        return -1;
    }

    printf("\n");
    printf("Write SPI ENC eFuse done.\n");
    while (1)
        continue;
    return 0;
}

CONSOLE_CMD(efuse_d12x_spienc, cmd_efuse_do_d12x_spienc, "eFuse test example");
