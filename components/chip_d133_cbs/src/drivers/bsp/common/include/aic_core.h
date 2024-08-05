/*
 * Copyright (c) 2022, Artinchip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __AIC_CORE_H__
#define __AIC_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys_freq.h>
#include <rtconfig.h>
#include <aic_soc.h>
#include <aic_arch.h>
#include <aic_common.h>
#include <aic_reboot_reason.h>
#include <aic_io.h>
#include <aic_errno.h>
#include <aic_list.h>
#include <aic_log.h>
#warning "remove temp"
#include <core/core_rv32.h>
#include <drv/irq.h>

#ifdef __cplusplus
}
#endif

#warning "fix me"
enum irqreturn
{
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};
typedef enum irqreturn irqreturn_t;
typedef irqreturn_t (*irq_handler_t)(int, void *);
typedef irqreturn_t (*pin_irq_handler_t)(void *);

static inline void aicos_mdelay(unsigned long msecs)
{
    aic_udelay(msecs * 1000);
}

static inline void aicos_udelay(unsigned long usecs)
{
    aic_udelay(usecs);
}

static inline int aicos_request_irq(unsigned int irq, irq_handler_t handler, unsigned int flags,
                                 const char *name, void *data)
{
    drv_irq_register(irq, handler, data);
    drv_irq_enable(irq);

    return 0;
}

static inline void aicos_irq_enable(unsigned int irq)
{
    csi_irq_enable(irq);
}

static inline void aicos_irq_disable(unsigned int irq)
{
    csi_irq_disable(irq);
}

#endif /* __AIC_CORE_H__ */
