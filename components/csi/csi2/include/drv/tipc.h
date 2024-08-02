/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/tipc.h
 * @brief    Header File for TIPC Driver
 * @version  V1.0
 * @date     08. Mar 2020
 * @model    tipc
 ******************************************************************************/

#ifndef _DRV_TIPC_H_
#define _DRV_TIPC_H_

#include <stdint.h>
#include <stdbool.h>
#include <csi_core.h>
#include <drv/common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t ip;
    uint16_t dev_tag;
    uint8_t  idx;
} csi_tipcmap_t;

/**
  \brief       Config the tipc module properity
  \param[in]   dev Dev handle \ref csi_dev_t
  \param[in]   is_secure is secure or not
*/
csi_error_t csi_dev_secure_config(csi_dev_t *dev, bool is_secure);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIPC_H_ */
