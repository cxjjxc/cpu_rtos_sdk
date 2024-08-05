#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* ArtInChip Luban-Lite SDK Configuration */

/* Project options */

#define PRJ_DEFCONFIG_FILENAME "d13x_demo88-nor_baremetal_helloworld_defconfig"

/* -- Important: If following options have been changed, you need save & rerun menuconfig before changing any other options. */

#define PRJ_CHIP "d13x"
#define PRJ_BOARD "demo88-nor"
#define PRJ_KERNEL "baremetal"
#define PRJ_APP "helloworld"
#define PLATFORM_LUBANLITE

/* Application options */

/* Filesystem related */

#define AIC_USING_FS_IMAGE_0
#define AIC_USING_FS_IMAGE_TYPE_FATFS_FOR_0
#define AIC_FS_IMAGE_DIR_0 "packages/artinchip/lvgl-ui/aic_demo/dashboard_demo/lvgl_src/"
#define AIC_FS_IMAGE_NAME_0 "rodata.fatfs"
#define AIC_FATFS_AUTO_SIZE_FOR_0
#define AIC_USING_FS_IMAGE_TYPE_FATFS_CLUSTER_SIZE 8
#define AIC_FATFS_ENABLE_WRITE_IN_SPINOR

/* LVGL demo select related */

#define LPKG_USING_LVGL
#define LPKG_LVGL_PATH "/packages/multimedia/LVGL/LVGL"
#define LPKG_LVGL_THREAD_PRIO 20
#define LPKG_LVGL_THREAD_STACK_SIZE 4096
#define LPKG_LVGL_DISP_REFR_PERIOD 5
#define AIC_LVGL_DEMO
#define AIC_LVGL_DASHBOARD_DEMO
#define LV_COLOR_DEPTH 16
#define LV_CACHE_IMG_NUM 2
#define LVGL_STORAGE_PATH "/rodata/lvgl_data"

/* Chip options */

#define SOC_THEAD_SMART
#define PRJ_CUSTOM_LDS ""
#define AIC_CHIP_D13X
#define CACHE_LINE_SIZE 32
#define CPU_BASE 0x20000000
#define AIC_CMU_DRV
#define AIC_CMU_DRV_V11
#define AIC_CMU_DRV_VER "11"
#define AIC_GPIO_DRV
#define AIC_GPIO_DRV_V11
#define AIC_GPIO_DRV_VER "11"
#define AIC_SYSCFG_DRV
#define AIC_SYSCFG_DRV_V11
#define AIC_SYSCFG_DRV_VER "11"
#define AIC_DMA_DRV
#define AIC_DMA_DRV_V11
#define AIC_DMA_DRV_VER "11"
#define AIC_DMA_CH_NUM 8
#define AIC_DMA_ALIGN_SIZE 8
#define AIC_UART_DRV
#define AIC_UART_DRV_V11
#define AIC_UART_DEV_NUM 8
#define AIC_QSPI_DRV
#define AIC_QSPI_DRV_V11
#define AIC_QSPI_DRV_VER "11"
#define AIC_XSPI_DRV
#define AIC_XSPI_DRV_V10
#define AIC_WRI_DRV
#define AIC_WRI_DRV_V11
#define AIC_WRI_DRV_VER "11"
#define AIC_RTC_DRV
#define AIC_RTC_DRV_V11
#define AIC_RTC_DRV_VER "11"
#define AIC_WDT_DRV
#define AIC_WDT_DRV_V10
#define AIC_WDT_DRV_VER "10"
#define AIC_SDMC_DRV
#define AIC_SDMC_DRV_V11
#define AIC_SDMC_DRV_VER "11"
#define AIC_DE_DRV
#define AIC_DE_DRV_V11
#define AIC_DE_DRV_VER "11"
#define AIC_GE_DRV
#define AIC_GE_DRV_V11
#define AIC_GE_DRV_VER "11"
#define AIC_VE_DRV
#define AIC_VE_DRV_V30
#define AIC_VE_DRV_VER "30"
#define AIC_AUDIO_DRV
#define AIC_AUDIO_DRV_V11
#define AIC_AUDIO_DRV_VER "11"

/* Board options */

/* Interface Related:  */

#define AIC_USING_UART0

/* Uart0 Parameter */

#define AIC_CLK_UART0_FREQ 48000000
#define AIC_DEV_UART0_BAUDRATE 115200
#define AIC_DEV_UART0_DATABITS 8
#define AIC_DEV_UART0_STOPBITS 1
#define AIC_DEV_UART0_PARITY 0
#define AIC_DEV_UART0_MODE_RS232
#define AIC_DEV_UART0_MODE 0
#define AIC_UART0_FLAG 259
#define AIC_DEV_UART0_RX_MODE_POLL
#define AIC_DEV_UART0_RX_MODE 0

/* Using EPWM */


/* Using HRTIMER */


/* Using CAP */


/* Storage Related:  */

#define AIC_USING_QSPI0

/* SPI0 Parameter */

#define AIC_DEV_QSPI0_MAX_SRC_FREQ_HZ 100000000
#define AIC_QSPI0_BUS_WIDTH_4
#define AIC_QSPI0_BUS_WIDTH 4
#define AIC_DEV_QSPI0_DELAY_MODE 2
#define AIC_QSPI0_DEVICE_SPINOR
#define AIC_QSPI0_DEVICE_SPINOR_FREQ 100000000
#define AIC_USING_SDMC1

/* SDMC1 Parameter */

#define AIC_SDMC1_BUSWIDTH4
#define AIC_SDMC1_DRV_PHASE 3
#define AIC_SDMC1_SMP_PHASE 0

/* WLAN Related: */

#define AIC_WIRELESS_LAN

/* Analog Related:  */


/* MutiMedia Related:  */

#define AIC_USING_DE

/* Display Parameter */

#define AIC_DISPLAY_DRV
#define AIC_DISP_DE_DRV
#define AIC_DISP_LVDS_DRV
#define AIC_DISP_LVDS
#define AIC_DI_TYPE 2

/* LVDS interface options */

#define AIC_LVDS_NS
#define AIC_LVDS_MODE 0
#define AIC_LVDS_LINK_0
#define AIC_LVDS_LINK_MODE 0
#define AICFB_RGB565
#define AICFB_FORMAT 0x0e
#define AIC_PAN_DISPLAY
#define AIC_DISP_COLOR_BLOCK
#define AICFB_ROTATE_0
#define AIC_FB_ROTATE_DEGREE 0

/* Display Panels */

#define AIC_SIMPLE_PANEL

/* display timing of simple panel */

#define PANEL_PIXELCLOCK 52
#define PANEL_HACTIVE 1024
#define PANEL_VACTIVE 600
#define PANEL_HBP 160
#define PANEL_HFP 160
#define PANEL_HSW 20
#define PANEL_VBP 12
#define PANEL_VFP 20
#define PANEL_VSW 2
#define AIC_GPIO_BACKLIGHT
#define AIC_PANEL_ENABLE_GPIO "PE.13"
#define AIC_USING_GE

/* Graphics Engine Parameter */

#define AIC_GE_NORMAL
#define AIC_GE_DITHER
#define AIC_USING_VE

/* Camera Support */

#define AIC_USING_AUDIO

/* Audio Parameter */

#define AIC_AUDIO_PLAYBACK
#define AIC_AUDIO_PA_ENABLE_GPIO "PD.10"
#define AIC_AUDIO_EN_PIN_LOW
#define AIC_AUDIO_SPK_1
#define AIC_AUDIO_DMIC

/* System Related:  */

#define AIC_USING_DMA
#define AIC_USING_RTC

/* RTC Parameter */

#define AIC_RTC_CLK_RATE 3276800
#define AIC_USING_WDT

/* Mem Options */

/* SRAM parameter */

#define AIC_SRAM_TOTAL_SIZE 0x100000
#define AIC_ITCM_SIZE 0x0
#define AIC_DTCM_SIZE 0x0
#define AIC_SRAM_S1_SIZE_0K
#define AIC_SRAM_S1_SIZE 0
#define AIC_SRAM_S1_REG_SIZE 0x00
#define AIC_SRAM1_SW_SIZE 0x0
#define AIC_BOOTLOADER_RESERVE_SIZE 0x40000

/* PSRAM parameter */

#define AIC_PSRAM_SIZE 0x800000
#define AIC_PSRAM_CMA_EN
#define AIC_PSRAM_SW_SIZE 0x0

/* ELF Sections memory location */

#define AIC_SEC_TEXT_PSRAM
#define AIC_SEC_RODATA_PSRAM
#define AIC_SEC_DATA_PSRAM
#define AIC_SEC_BSS_PSRAM
#define AIC_INTERRUPTSTACK_SIZE 4096

/* Clocks options */

#define AIC_CLK_PLL_INT0_FREQ 480000000
#define AIC_CLK_PLL_INT1_FREQ 1200000000
#define AIC_CLK_PLL_FRA0_FREQ 792000000
#define AIC_CLK_PLL_FRA2_FREQ 1188000000
#define AIC_CLK_CPU_FREQ 480000000
#define AIC_CLK_AXI0_FREQ 200000000
#define AIC_CLK_AHB0_FREQ 200000000
#define AIC_CLK_APB0_FREQ 100000000

/* Security Related:  */

#define AIC_USING_SYSCFG

/* Syscfg Parameter */

#define AIC_SYSCFG_SIP_FLASH_ENABLE
#define AIC_SIP_FLASH_ACCESS_QSPI_ID 0

/* Baremetal options */

#define KERNEL_BAREMETAL
#define DRIVER_HAL_EN
#define DRIVER_BARE_DRV_EN
#define AIC_NORMALSTACK_SIZE 8092
#define AIC_BAREMETAL_CONSOLE_UART 0
#define ARCH_RISCV
#define ARCH_RISCV_FPU
#define ARCH_RISCV_FPU_D
#define ARCH_RISCV32

/* Local packages options */

/* Third-party packages options */

#define LPKG_USING_DFS
#define DFS_USING_POSIX
#define DFS_FILESYSTEMS_MAX 8
#define DFS_FILESYSTEM_TYPES_MAX 4
#define DFS_FD_MAX 16
#define LPKG_USING_DFS_ELMFAT

/* elm-chan's FatFs, Generic FAT Filesystem Module */

#define RT_DFS_ELM_CODE_PAGE 437
#define RT_DFS_ELM_WORD_ACCESS
#define LPKG_DFS_ELM_USE_LFN_3
#define RT_DFS_ELM_USE_LFN 3
#define LPKG_DFS_ELM_LFN_UNICODE_0
#define RT_DFS_ELM_LFN_UNICODE 0
#define RT_DFS_ELM_USE_EXFAT
#define RT_DFS_ELM_MAX_LFN 255
#define RT_DFS_ELM_DRIVES 2
#define RT_DFS_ELM_MAX_SECTOR_SIZE 512
#define RT_DFS_FAL_BLK_DEVICE_NANE "rodata"
#define RT_DFS_ELM_REENTRANT
#define RT_DFS_ELM_MUTEX_TIMEOUT 3000
#define LPKG_USING_DFS_ROMFS

/* ArtInChip packages options */

#define LPKG_MPP
#define LPKG_USING_ENV

/* Drivers options */

/* AIC Bare Driver */

#define AIC_MTD_BARE_DRV
#define AIC_CONSOLE_BARE_DRV
#define AIC_CONSOLE_SYSNAME "aic"
#define AIC_PRINTF_BARE_DRV
#define AIC_USING_TLSF_HEAP
#define TLSF_MEM_HEAP
#define AIC_GPIO_IRQ_DRV_EN
#define AIC_SPINOR_DRV

/* Peripheral */

#define LPKG_USING_SFUD

/* Touch Panel Support */

/* Gt911 touch panel options */


/* Ft7411 touch panel options */


/* GSL1680 touch panel options */


/* RTP touch panel options */


/* St16xx touch panel options */


/* Axs15260 touch panel options */


/* External Audio Codec Support */


/* Drivers debug */

#define AIC_LOG_LEVEL_WARN
#define AIC_LOG_LEVEL 4

/* Drivers examples */

#define AIC_MTD_BARE_TEST
#define AIC_AUDIO_DRV_TEST

#endif
