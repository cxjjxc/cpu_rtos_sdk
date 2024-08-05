/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023-2024, ArtInChip Technology Co., Ltd
 *
 * Authors:
 *   dwj <weijie.ding@aic.com>
 */

#define PRCM_SCSS_CPU_STATUS        0x88000110
#define PRCM_SC_CONTEXT_ADDR        0x8800011c

    .section .entry, "ax", %progbits
    .align  3
    .option pic
    .global sc_save_context_and_suspend
sc_save_context_and_suspend:
    sw      ra, 0(a0)
    sw      sp, 4(a0)
    sw      gp, 8(a0)
    sw      tp, 0xc(a0)
    sw      s0, 0x10(a0)
    sw      s1, 0x14(a0)
    sw      a1, 0x18(a0)
    sw      a2, 0x1c(a0)
    sw      a3, 0x20(a0)
    sw      a4, 0x24(a0)
    sw      a5, 0x28(a0)
    sw      a6, 0x2c(a0)
    sw      a7, 0x30(a0)
    sw      s2, 0x34(a0)
    sw      s3, 0x38(a0)
    sw      s4, 0x3c(a0)
    sw      s5, 0x40(a0)
    sw      s6, 0x44(a0)
    sw      s7, 0x48(a0)
    sw      s8, 0x4c(a0)
    sw      s9, 0x50(a0)
    sw      s10, 0x54(a0)
    sw      s11, 0x58(a0)

    /* save CSRs */
    csrr    t0, mepc
    sw      t0, 0x5c(a0)
    csrr    t0, mstatus
    sw      t0, 0x60(a0)
    csrr    t0, mtval
    sw      t0, 0x64(a0)
    csrr    t0, mcause
    sw      t0, 0x68(a0)
    csrr    t0, mie
    sw      t0, 0x6c(a0)
    csrr    t0, mtvec
    sw      t0, 0x70(a0)
    csrr    t0, mtvt
    sw      t0, 0x74(a0)
    csrr    t0, mscratch
    sw      t0, 0x78(a0)
    csrr    t0, mxstatus
    sw      t0, 0x7c(a0)
    csrr    t0, mhcr
    sw      t0, 0x80(a0)
    csrr    t0, mhint
    sw      t0, 0x84(a0)
    csrr    t0, mexstatus
    sw      t0, 0x88(a0)

    /* save sc context addr to PRCM_SC_CONTEXT_ADDR */
    li      t0, PRCM_SC_CONTEXT_ADDR
    sw      a0, (t0)

    /* Update CPU status to PRCM bank register */
    li      t0, PRCM_SCSS_CPU_STATUS
    li      t1, 1
    sw      t1, (t0)

    fence.i
    fence
    sync.i
    sync
    wfi

    ret
sc_save_context_and_suspend_end:

    .data
    .align  3
    .global sc_save_context_and_suspend_size
sc_save_context_and_suspend_size:
    .word   sc_save_context_and_suspend_end - sc_save_context_and_suspend

    .section .entry, "ax", %progbits
    .align  3
    .option pic
    .global sc_restore_context_and_resume
sc_restore_context_and_resume:
    li      t0, PRCM_SC_CONTEXT_ADDR
    lw      a0, (t0)

    lw      ra, 0(a0)
    lw      sp, 4(a0)
    lw      gp, 8(a0)
    lw      tp, 0xc(a0)
    lw      s0, 0x10(a0)
    lw      s1, 0x14(a0)
    lw      a1, 0x18(a0)
    lw      a2, 0x1c(a0)
    lw      a3, 0x20(a0)
    lw      a4, 0x24(a0)
    lw      a5, 0x28(a0)
    lw      a6, 0x2c(a0)
    lw      a7, 0x30(a0)
    lw      s2, 0x34(a0)
    lw      s3, 0x38(a0)
    lw      s4, 0x3c(a0)
    lw      s5, 0x40(a0)
    lw      s6, 0x44(a0)
    lw      s7, 0x48(a0)
    lw      s8, 0x4c(a0)
    lw      s9, 0x50(a0)
    lw      s10, 0x54(a0)
    lw      s11, 0x58(a0)

    /* restore CSRs */
    lw      t0, 0x5c(a0)
    csrw    mepc, t0
    lw      t0, 0x60(a0)
    csrw    mstatus, t0
    lw      t0, 0x64(a0)
    csrw    mtval, t0
    lw      t0, 0x68(a0)
    csrw    mcause, t0
    lw      t0, 0x6c(a0)
    csrw    mie, t0
    lw      t0, 0x70(a0)
    csrw    mtvec, t0
    lw      t0, 0x74(a0)
    csrw    mtvt, t0
    lw      t0, 0x78(a0)
    csrw    mscratch, t0
    lw      t0, 0x7c(a0)
    csrw    mxstatus, t0
    lw      t0, 0x80(a0)
    csrw    mhcr, t0
    lw      t0, 0x84(a0)
    csrw    mhint, t0
    lw      t0, 0x88(a0)
    csrw    mexstatus, t0

    /* clear sc context addr to 0 in PRCM_SC_CONTEXT_ADDR */
    li      t0, PRCM_SC_CONTEXT_ADDR
    sw      zero, (t0)

    /* clear CPU status to PRCM bank register */
    li      t0, PRCM_SCSS_CPU_STATUS
    sw      zero, (t0)

    ret
sc_restore_context_and_resume_end:

    .data
    .align  3
    .global sc_restore_context_and_resume_size
sc_restore_context_and_resume_size:
    .word   sc_restore_context_and_resume_end - sc_restore_context_and_resume
