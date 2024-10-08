/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018/10/28     Bernard      The unify RISC-V porting code.
 * 2021-02-11     lizhirui     add gp support
 * 2021-11-19     JasonHu      add fpu support
 */

#include <rthw.h>
#include <rtthread.h>
#include <stddef.h>
#include <asm/riscv_csr.h>
#include "cpuport.h"
#include "stack.h"

/**
 * @brief from thread used interrupt context switch
 *
 */
volatile rt_ubase_t  rt_interrupt_from_thread = 0;
/**
 * @brief to thread used interrupt context switch
 *
 */
volatile rt_ubase_t  rt_interrupt_to_thread   = 0;
/**
 * @brief flag to indicate context switch in interrupt or not
 *
 */
volatile rt_ubase_t rt_thread_switch_interrupt_flag = 0;


/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
rt_uint8_t *rt_hw_stack_init(void       *tentry,
                             void       *parameter,
                             rt_uint8_t *stack_addr,
                             void       *texit)
{
    struct rt_hw_stack_frame *frame;
    rt_uint8_t         *stk;
    int                i;
    extern int __global_pointer$;

    stk  = stack_addr + sizeof(rt_ubase_t);
    stk  = (rt_uint8_t *)RT_ALIGN_DOWN((rt_ubase_t)stk, REGBYTES);
    stk -= sizeof(struct rt_hw_stack_frame);

    frame = (struct rt_hw_stack_frame *)stk;

    for (i = 0; i < sizeof(struct rt_hw_stack_frame) / sizeof(rt_ubase_t); i++) {
        ((rt_ubase_t *)frame)[i] = i;
    }

    frame->ra      = (rt_ubase_t)texit;
    frame->gp      = (rt_ubase_t)&__global_pointer$;
    frame->a0      = (rt_ubase_t)parameter;
    frame->epc     = (rt_ubase_t)tentry;
    frame->x2      = (rt_ubase_t)stk;

    frame->mstatus = SR_SUM | SR_MPP_M | SR_MPIE;
#if defined(__riscv_flen) && defined(ENABLE_FPU)
    frame->mstatus |= SR_FS_INITIAL;    /* enable FPU */
#endif

    return stk;
}
/*
 * #ifdef RT_USING_SMP
 * void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread);
 * #else
 * void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to);
 * #endif
 */
// #ifndef RT_USING_SMP
// void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to, rt_thread_t from_thread, rt_thread_t to_thread)
// {
//     if (rt_thread_switch_interrupt_flag == 0)
//         rt_interrupt_from_thread = from;

//     rt_interrupt_to_thread = to;
//     rt_thread_switch_interrupt_flag = 1;

//     // rt_trigger_software_interrupt();
//     return ;
// }

// #endif /* end of RT_USING_SMP */

int rt_hw_cpu_id(void)
{
    return 0;   /* d1 has one core */
}

void rt_hw_set_process_id(int pid)
{
    //TODO
}

rt_bool_t rt_hw_interrupt_is_disabled(void)
{
    extern unsigned long cpu_is_irq_enable();
    return !cpu_is_irq_enable();
}
