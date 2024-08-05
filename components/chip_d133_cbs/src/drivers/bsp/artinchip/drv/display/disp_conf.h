/*
 * Copyright (c) 2023, Artinchip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _DISP_CONF_H_
#define _DISP_CONF_H_

/**
 * LVDS options
 */
#include <aic_hal_lvds.h>

/* lvds sync mode enable */
#define AIC_LVDS_SYNC_MODE_EN   1

/* lvds link swap enable, swap lvds link0 and link1 */
#define AIC_LVDS_LINK_SWAP_EN   0

/**
 * lvds channel output order
 *
 * default D3 CK D2 D1 D0
 *          4  3  2  1  0
 */
#define AIC_LVDS_LINK0_LANES      LVDS_LANES(LVDS_D3, LVDS_CK, LVDS_D2, LVDS_D1, LVDS_D0)
#define AIC_LVDS_LINK1_LANES      LVDS_LANES(LVDS_D3, LVDS_CK, LVDS_D2, LVDS_D1, LVDS_D0)

/**
 * lvds channel polarities
 */
#define AIC_LVDS_LINK0_POL        0x0
#define AIC_LVDS_LINK1_POL        0x0

/**
 * lvds channel phy config
 */
#define AIC_LVDS_LINK0_PHY        0xFA
#define AIC_LVDS_LINK1_PHY        0xFA

/**
 * MIPI-DSI options
 */

/* data line assignments */
#define LANE_ASSIGNMENTS 0x3210

/* data line polarities */
#define LANE_POLARITIES  0b0000

/* data clk inverse */
#define CLK_INVERSE      0

/* virtual channel id */
#define VIRTUAL_CHANNEL  0

/* mipi-dsi lp rate, range [10M, 20M], default 10M */
#define MIPI_DSI_LP_RATE    (10 * 1000 * 1000)

/* mipi-dsi dcs get display id from screen when panel enable */
#define DCS_GET_DISPLAY_ID  0

/**
 * FB ROTATION options
 */

/* drawing buf for GUI, range [1, 2] */
#define AIC_FB_DRAW_BUF_NUM 2

/**
 * Display Engine options
 */

/* display flags, config hsync/vsync polarities */
#define AIC_DISPLAY_FLAGS       (DISPLAY_FLAGS_HSYNC_LOW | DISPLAY_FLAGS_VSYNC_LOW)

/**
 * Display Engine Mode
 *
 * Continue mode, ignore the TE signal of LCD and the timing signal
 * of display engine is continuous.
 *
 * Single frame mode, the timing signal of display engine needs to be
 * manually updated.
 *
 * Auto mode, need a TE pulse width. The display engine automatically
 * updates timing signal after obtained a TE signal from LCD.
 *
 * If unsure, say continuous mode.
 */

#define CONTINUE    0
#define SINGLE      1
#define AUTO        2

#define DE_MODE     CONTINUE

/**
 * AUTO mode options
 */
#if ( DE_MODE == 2 )
# define DE_AUTO_MODE 1
#endif

#if DE_AUTO_MODE
/**
 * TE PIN
 *
 * D12x, just support { "PC.6", "PD.2", "PF.15" }
 * D13x, just support { "PC.6", "PA.1" }
 */
#  define TE_PIN      "PC.6"
#  define TE_PULSE_WIDTH  2
#endif

#ifdef AIC_DISP_PQ_TOOL

#define PANEL_PIXELCLOCK      70
#define PANEL_HACTIVE         800
#define PANEL_HBP             150
#define PANEL_HFP             160
#define PANEL_HSW             20
#define PANEL_VACTIVE         1280
#define PANEL_VBP             12
#define PANEL_VFP             20
#define PANEL_VSW             2

#define AIC_RGB_MODE          PRGB
#define AIC_RGB_FORMAT        PRGB_18BIT_HD
#define AIC_RGB_CLK_CTL       DEGREE_0
#define AIC_RGB_DATA_ORDER    RGB
#define AIC_RGB_DATA_MIRROR   0

#define AIC_LVDS_MODE         NS
#define AIC_LVDS_LINK_MODE    SINGLE_LINK0

#define AIC_MIPI_DSI_MODE     DSI_MOD_VID_BURST
#define AIC_MIPI_DSI_FORMAT   DSI_FMT_RGB888
#define AIC_MIPI_DSI_LINE_NUM 4

#define AIC_PANEL_ENABLE_GPIO "PE.19"

#define PANEL_RESET 0
#if PANEL_RESET
#define AIC_PANEL_RESET_GPIO "PE.6"
#endif

struct disp_pinmux {
    unsigned char func;
    unsigned char bias;
    unsigned char drive;
    char *name;
};

#define AIC_PQ_TOOL_SET_DISP_PINMUX_FOPS(disp_pin)   \
    static void aic_set_disp_pinmux_for_pq_tool(void)\
    {\
        uint32_t i = 0;\
        long pin = 0;\
        unsigned int g;\
        unsigned int p;\
                        \
        for (i = 0; i < ARRAY_SIZE(disp_pin); i++) {\
            pin = hal_gpio_name2pin(disp_pin[i].name);\
            if (pin < 0)\
                continue;\
            g = GPIO_GROUP(pin);\
            p = GPIO_GROUP_PIN(pin);\
            hal_gpio_set_func(g, p, disp_pin[i].func);\
            hal_gpio_set_bias_pull(g, p, disp_pin[i].bias);\
            hal_gpio_set_drive_strength(g, p, disp_pin[i].drive);\
        }\
    }

#define AIC_PQ_SET_DSIP_PINMUX    \
    aic_set_disp_pinmux_for_pq_tool()

#define AIC_PQ_TOOL_PINMUX_CONFIG(name)     \
    static struct disp_pinmux name[] = { \
        {3, PIN_PULL_DIS, 3, "PD.8"},\
        {3, PIN_PULL_DIS, 3, "PD.9"},\
        {3, PIN_PULL_DIS, 3, "PD.10"},\
        {3, PIN_PULL_DIS, 3, "PD.11"},\
        {3, PIN_PULL_DIS, 3, "PD.12"},\
        {3, PIN_PULL_DIS, 3, "PD.13"},\
        {3, PIN_PULL_DIS, 3, "PD.14"},\
        {3, PIN_PULL_DIS, 3, "PD.15"},\
        {3, PIN_PULL_DIS, 3, "PD.16"},\
        {3, PIN_PULL_DIS, 3, "PD.17"},\
    }

#define PANEL_DSI_SIMEP_SEND_SEQ                \
    panel_dsi_dcs_send_seq(panel, 0x00);        \

#endif

#endif /* _DISP_CONF_H_ */
