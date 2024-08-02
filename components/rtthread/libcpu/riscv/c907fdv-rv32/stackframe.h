/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-02     lizhirui     first version
 * 2021-02-11     lizhirui     fixed gp save/store bug
 * 2021-11-18     JasonHu      add fpu registers save/restore
 */

#ifndef __STACKFRAME_H__
#define __STACKFRAME_H__

#include "cpuport.h"

#if defined(__riscv_flen) && defined(ENABLE_FPU)
#if __riscv_flen == 64
#define FSTORE fsd
#define FLOAD  fld
#define FREGBYTES 8
#elif __riscv_flen == 32
#define FSTORE fsw
#define FLOAD  flw
#define FREGBYTES 4
#endif
#define FPU_CTX_F0_OFF   (__riscv_flen >> 5) * 0   /* offsetof(fpu_context_t, fpustatus.f[0])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F1_OFF   (__riscv_flen >> 5) * 4   /* offsetof(fpu_context_t, fpustatus.f[1])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F2_OFF   (__riscv_flen >> 5) * 8   /* offsetof(fpu_context_t, fpustatus.f[2])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F3_OFF   (__riscv_flen >> 5) * 12  /* offsetof(fpu_context_t, fpustatus.f[3])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F4_OFF   (__riscv_flen >> 5) * 16  /* offsetof(fpu_context_t, fpustatus.f[4])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F5_OFF   (__riscv_flen >> 5) * 20  /* offsetof(fpu_context_t, fpustatus.f[5])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F6_OFF   (__riscv_flen >> 5) * 24  /* offsetof(fpu_context_t, fpustatus.f[6])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F7_OFF   (__riscv_flen >> 5) * 28  /* offsetof(fpu_context_t, fpustatus.f[7])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F8_OFF   (__riscv_flen >> 5) * 32  /* offsetof(fpu_context_t, fpustatus.f[8])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F9_OFF   (__riscv_flen >> 5) * 36  /* offsetof(fpu_context_t, fpustatus.f[9])  - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F10_OFF  (__riscv_flen >> 5) * 40  /* offsetof(fpu_context_t, fpustatus.f[10]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F11_OFF  (__riscv_flen >> 5) * 44  /* offsetof(fpu_context_t, fpustatus.f[11]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F12_OFF  (__riscv_flen >> 5) * 48  /* offsetof(fpu_context_t, fpustatus.f[12]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F13_OFF  (__riscv_flen >> 5) * 52  /* offsetof(fpu_context_t, fpustatus.f[13]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F14_OFF  (__riscv_flen >> 5) * 56  /* offsetof(fpu_context_t, fpustatus.f[14]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F15_OFF  (__riscv_flen >> 5) * 60  /* offsetof(fpu_context_t, fpustatus.f[15]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F16_OFF  (__riscv_flen >> 5) * 64  /* offsetof(fpu_context_t, fpustatus.f[16]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F17_OFF  (__riscv_flen >> 5) * 68  /* offsetof(fpu_context_t, fpustatus.f[17]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F18_OFF  (__riscv_flen >> 5) * 72  /* offsetof(fpu_context_t, fpustatus.f[18]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F19_OFF  (__riscv_flen >> 5) * 76  /* offsetof(fpu_context_t, fpustatus.f[19]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F20_OFF  (__riscv_flen >> 5) * 80  /* offsetof(fpu_context_t, fpustatus.f[20]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F21_OFF  (__riscv_flen >> 5) * 84  /* offsetof(fpu_context_t, fpustatus.f[21]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F22_OFF  (__riscv_flen >> 5) * 88  /* offsetof(fpu_context_t, fpustatus.f[22]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F23_OFF  (__riscv_flen >> 5) * 92  /* offsetof(fpu_context_t, fpustatus.f[23]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F24_OFF  (__riscv_flen >> 5) * 96  /* offsetof(fpu_context_t, fpustatus.f[24]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F25_OFF  (__riscv_flen >> 5) * 100 /* offsetof(fpu_context_t, fpustatus.f[25]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F26_OFF  (__riscv_flen >> 5) * 104 /* offsetof(fpu_context_t, fpustatus.f[26]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F27_OFF  (__riscv_flen >> 5) * 108 /* offsetof(fpu_context_t, fpustatus.f[27]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F28_OFF  (__riscv_flen >> 5) * 112 /* offsetof(fpu_context_t, fpustatus.f[28]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F29_OFF  (__riscv_flen >> 5) * 116 /* offsetof(fpu_context_t, fpustatus.f[29]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F30_OFF  (__riscv_flen >> 5) * 120 /* offsetof(fpu_context_t, fpustatus.f[30]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#define FPU_CTX_F31_OFF  (__riscv_flen >> 5) * 124 /* offsetof(fpu_context_t, fpustatus.f[31]) - offsetof(fpu_context_t, fpustatus.f[0]) */
#endif /* ENABLE_FPU */

.macro SAVE_ALL
    /* save general registers */
    addi sp, sp, -CTX_GENERAL_REG_NR * REGBYTES
    STORE x1,   0 * REGBYTES(sp)
    STORE x3,   2 * REGBYTES(sp)
    STORE x4,   3 * REGBYTES(sp)
    STORE x5,   4 * REGBYTES(sp)
    STORE x6,   5 * REGBYTES(sp)
    STORE x7,   6 * REGBYTES(sp)
    STORE x8,   7 * REGBYTES(sp)
    STORE x9,   8 * REGBYTES(sp)
    STORE x10,  9 * REGBYTES(sp)
    STORE x11, 10 * REGBYTES(sp)
    STORE x12, 11 * REGBYTES(sp)
    STORE x13, 12 * REGBYTES(sp)
    STORE x14, 13 * REGBYTES(sp)
    STORE x15, 14 * REGBYTES(sp)
    STORE x16, 15 * REGBYTES(sp)
    STORE x17, 16 * REGBYTES(sp)
    STORE x18, 17 * REGBYTES(sp)
    STORE x19, 18 * REGBYTES(sp)
    STORE x20, 19 * REGBYTES(sp)
    STORE x21, 20 * REGBYTES(sp)
    STORE x22, 21 * REGBYTES(sp)
    STORE x23, 22 * REGBYTES(sp)
    STORE x24, 23 * REGBYTES(sp)
    STORE x25, 24 * REGBYTES(sp)
    STORE x26, 25 * REGBYTES(sp)
    STORE x27, 26 * REGBYTES(sp)
    STORE x28, 27 * REGBYTES(sp)
    STORE x29, 28 * REGBYTES(sp)
    STORE x30, 29 * REGBYTES(sp)
    STORE x31, 30 * REGBYTES(sp)
    csrr  x1, mepc
    STORE x1,  31 * REGBYTES(sp)
    csrr  x1, mstatus
    STORE x1,  32 * REGBYTES(sp)

#if defined(__riscv_flen) && defined(ENABLE_FPU)
    /* save fcsr registers */
    addi sp, sp, -(CTX_FPU_CSR_REG_NR) * FREGBYTES
    frcsr    t0
    STORE    t0, (0)(sp)
    /* save fpu registers*/
    addi sp, sp, -(CTX_FPU_REG_NR) * FREGBYTES
    FSTORE f0,  FPU_CTX_F0_OFF(sp)
    FSTORE f1,  FPU_CTX_F1_OFF(sp)
    FSTORE f2,  FPU_CTX_F2_OFF(sp)
    FSTORE f3,  FPU_CTX_F3_OFF(sp)
    FSTORE f4,  FPU_CTX_F4_OFF(sp)
    FSTORE f5,  FPU_CTX_F5_OFF(sp)
    FSTORE f6,  FPU_CTX_F6_OFF(sp)
    FSTORE f7,  FPU_CTX_F7_OFF(sp)
    FSTORE f8,  FPU_CTX_F8_OFF(sp)
    FSTORE f9,  FPU_CTX_F9_OFF(sp)
    FSTORE f10, FPU_CTX_F10_OFF(sp)
    FSTORE f11, FPU_CTX_F11_OFF(sp)
    FSTORE f12, FPU_CTX_F12_OFF(sp)
    FSTORE f13, FPU_CTX_F13_OFF(sp)
    FSTORE f14, FPU_CTX_F14_OFF(sp)
    FSTORE f15, FPU_CTX_F15_OFF(sp)
    FSTORE f16, FPU_CTX_F16_OFF(sp)
    FSTORE f17, FPU_CTX_F17_OFF(sp)
    FSTORE f18, FPU_CTX_F18_OFF(sp)
    FSTORE f19, FPU_CTX_F19_OFF(sp)
    FSTORE f20, FPU_CTX_F20_OFF(sp)
    FSTORE f21, FPU_CTX_F21_OFF(sp)
    FSTORE f22, FPU_CTX_F22_OFF(sp)
    FSTORE f23, FPU_CTX_F23_OFF(sp)
    FSTORE f24, FPU_CTX_F24_OFF(sp)
    FSTORE f25, FPU_CTX_F25_OFF(sp)
    FSTORE f26, FPU_CTX_F26_OFF(sp)
    FSTORE f27, FPU_CTX_F27_OFF(sp)
    FSTORE f28, FPU_CTX_F28_OFF(sp)
    FSTORE f29, FPU_CTX_F29_OFF(sp)
    FSTORE f30, FPU_CTX_F30_OFF(sp)
    FSTORE f31, FPU_CTX_F31_OFF(sp)
#endif /* __riscv_flen && ENABLE_FPU */

#if defined(__riscv_vector) && defined(ENABLE_VECTOR)
    /* save vector csr registers */
    addi sp, sp, -(CTX_VECTOR_CSR_REG_NR) * REGBYTES
    csrr    t0, vl
    STORE   t0, (0)(sp)
    csrr    t0, vtype
    STORE   t0, (1 * REGBYTES)(sp)
    csrr    t0, vstart
    STORE   t0, (2 * REGBYTES)(sp)
    csrr    t0, vxsat
    STORE   t0, (3 * REGBYTES)(sp)
    csrr    t0, vxrm
    STORE   t0, (4 * REGBYTES)(sp)
    /* save vector registers */
    csrr t0, vlenb
    li   t1, CTX_VECTOR_REG_NR
    mul  t2, t0, t1
    sub  sp, sp, t2
    slli t0, t0, 3
    mv   t1, sp
    vsetvli  zero, zero, e8, m8
#if (__riscv_v == 7000)
    vsb.v    v0, (t1)
    add      t1, t1, t0
    vsb.v    v8, (t1)
    add      t1, t1, t0
    vsb.v    v16, (t1)
    add      t1, t1, t0
    vsb.v    v24, (t1)
#elif (__riscv_v == 1000000)
    vs8r.v   v0, (t1)
    add      t1, t1, t0
    vs8r.v   v8, (t1)
    add      t1, t1, t0
    vs8r.v   v16, (t1)
    add      t1, t1, t0
    vs8r.v   v24, (t1)
#endif
#endif /* __riscv_vector && ENABLE_VECTOR */

#if defined(__riscv_matrix)
    /* save matrix csr registers */
    addi sp, sp, -(CTX_MATRIX_CSR_REG_NR) * REGBYTES
    csrr    t0, xmrstart
    STORE   t0,  (0)(sp)
    csrr    t0, xmcsr
    STORE   t0,  (1 * REGBYTES)(sp)
    csrr    t0, xmsize
    STORE   t0,  (2 * REGBYTES)(sp)
    /* save matrix registers */
    csrr    t0, xmlenb
    slli    t1, t0, 3
    sub     sp, sp, t1
    csrw    xmrstart, x0
    mst8mb  m0, (sp)
#endif /* __riscv_matrix */
.endm

.macro RESTORE_ALL
#if defined(__riscv_matrix)
    /* restore matrix registers */
    csrr     t0, xmlenb
    slli     t1, t0, 3
    csrw     xmrstart, x0
    mst8mb   m0, (sp)
    add      sp, sp, t1
    /* restore matrix csr registers */
    csrr    t0, xmrstart
    LOAD    t0, (0)(sp)
    csrr    t0, xmcsr
    LOAD    t0, (1 * REGBYTES)(sp)
    csrr    t0, xmsize
    LOAD    t0, (2 * REGBYTES)(sp)
    addi sp, sp, (CTX_MATRIX_CSR_REG_NR) * REGBYTES
#endif /* __riscv_matrix */

#if defined(__riscv_vector) && defined(ENABLE_VECTOR)
    /* restore vector registers */
    csrr     t0, vlenb
    slli     t0, t0, 3
    vsetvli  zero, zero, e8, m8
#if (__riscv_v == 7000)
    vlb.v    v0, (sp)
    add      sp, sp, t0
    vlb.v    v8, (sp)
    add      sp, sp, t0
    vlb.v    v16, (sp)
    add      sp, sp, t0
    vlb.v    v24, (sp)
    add      sp, sp, t0
#elif (__riscv_v == 1000000)
    vl8r.v   v0, (sp)
    add      sp, sp, t0
    vl8r.v   v8, (sp)
    add      sp, sp, t0
    vl8r.v   v16, (sp)
    add      sp, sp, t0
    vl8r.v   v24, (sp)
    add      sp, sp, t0
#endif
    /* restore vector csr registers */
    LOAD     t0, (0)(sp)
    LOAD     t1, (1 * REGBYTES)(sp)
    LOAD     t2, (2 * REGBYTES)(sp)
    vsetvl   zero, t0, t1
    csrw     vstart, t2
    LOAD     t2, (3 * REGBYTES)(sp)
    csrw     vxsat, t2
    LOAD     t2, (4 * REGBYTES)(sp)
    csrw     vxrm, t2
    addi     sp, sp, (CTX_VECTOR_CSR_REG_NR) * REGBYTES
#endif /* __riscv_vector && ENABLE_VECTOR */

#if defined(__riscv_flen) && defined(ENABLE_FPU)
    /* restore float register */
    FLOAD f0, FPU_CTX_F0_OFF(sp)
    FLOAD f1, FPU_CTX_F1_OFF(sp)
    FLOAD f2, FPU_CTX_F2_OFF(sp)
    FLOAD f3, FPU_CTX_F3_OFF(sp)
    FLOAD f4, FPU_CTX_F4_OFF(sp)
    FLOAD f5, FPU_CTX_F5_OFF(sp)
    FLOAD f6, FPU_CTX_F6_OFF(sp)
    FLOAD f7, FPU_CTX_F7_OFF(sp)
    FLOAD f8, FPU_CTX_F8_OFF(sp)
    FLOAD f9, FPU_CTX_F9_OFF(sp)
    FLOAD f10,FPU_CTX_F10_OFF(sp)
    FLOAD f11,FPU_CTX_F11_OFF(sp)
    FLOAD f12,FPU_CTX_F12_OFF(sp)
    FLOAD f13,FPU_CTX_F13_OFF(sp)
    FLOAD f14,FPU_CTX_F14_OFF(sp)
    FLOAD f15,FPU_CTX_F15_OFF(sp)
    FLOAD f16,FPU_CTX_F16_OFF(sp)
    FLOAD f17,FPU_CTX_F17_OFF(sp)
    FLOAD f18,FPU_CTX_F18_OFF(sp)
    FLOAD f19,FPU_CTX_F19_OFF(sp)
    FLOAD f20,FPU_CTX_F20_OFF(sp)
    FLOAD f21,FPU_CTX_F21_OFF(sp)
    FLOAD f22,FPU_CTX_F22_OFF(sp)
    FLOAD f23,FPU_CTX_F23_OFF(sp)
    FLOAD f24,FPU_CTX_F24_OFF(sp)
    FLOAD f25,FPU_CTX_F25_OFF(sp)
    FLOAD f26,FPU_CTX_F26_OFF(sp)
    FLOAD f27,FPU_CTX_F27_OFF(sp)
    FLOAD f28,FPU_CTX_F28_OFF(sp)
    FLOAD f29,FPU_CTX_F29_OFF(sp)
    FLOAD f30,FPU_CTX_F30_OFF(sp)
    FLOAD f31,FPU_CTX_F31_OFF(sp)
    addi     sp, sp, CTX_FPU_REG_NR * FREGBYTES
    /* restore fcsr registers */
    LOAD     t0, 0(sp)
    fscsr    t0
    addi     sp, sp, CTX_FPU_CSR_REG_NR * FREGBYTES
#endif /* __riscv_flen && ENABLE_FPU */

    /* restore general registers */
    LOAD x1,  31 * REGBYTES(sp)
    csrw mepc, x1
    LOAD x1,  32 * REGBYTES(sp)
    csrw mstatus, x1
    LOAD x1,   0 * REGBYTES(sp)
    LOAD x3,   2 * REGBYTES(sp)
    LOAD x4,   3 * REGBYTES(sp)
    LOAD x5,   4 * REGBYTES(sp)
    LOAD x6,   5 * REGBYTES(sp)
    LOAD x7,   6 * REGBYTES(sp)
    LOAD x8,   7 * REGBYTES(sp)
    LOAD x9,   8 * REGBYTES(sp)
    LOAD x10,  9 * REGBYTES(sp)
    LOAD x11, 10 * REGBYTES(sp)
    LOAD x12, 11 * REGBYTES(sp)
    LOAD x13, 12 * REGBYTES(sp)
    LOAD x14, 13 * REGBYTES(sp)
    LOAD x15, 14 * REGBYTES(sp)
    LOAD x16, 15 * REGBYTES(sp)
    LOAD x17, 16 * REGBYTES(sp)
    LOAD x18, 17 * REGBYTES(sp)
    LOAD x19, 18 * REGBYTES(sp)
    LOAD x20, 19 * REGBYTES(sp)
    LOAD x21, 20 * REGBYTES(sp)
    LOAD x22, 21 * REGBYTES(sp)
    LOAD x23, 22 * REGBYTES(sp)
    LOAD x24, 23 * REGBYTES(sp)
    LOAD x25, 24 * REGBYTES(sp)
    LOAD x26, 25 * REGBYTES(sp)
    LOAD x27, 26 * REGBYTES(sp)
    LOAD x28, 27 * REGBYTES(sp)
    LOAD x29, 28 * REGBYTES(sp)
    LOAD x30, 29 * REGBYTES(sp)
    LOAD x31, 30 * REGBYTES(sp)
    addi sp, sp, CTX_GENERAL_REG_NR * REGBYTES
.endm

.macro RESTORE_SYS_GP
    .option push
    .option norelax
        la gp, __global_pointer$
    .option pop
.endm

.macro OPEN_INTERRUPT
    csrsi mstatus, 8
.endm

.macro CLOSE_INTERRUPT
    csrci mstatus, 8
.endm

#endif
