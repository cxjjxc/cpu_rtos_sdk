/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

 /******************************************************************************
 * @file     startup.S
 * @brief    startup file.
 * @version  V1.0
 * @date     15. May 2020
 ******************************************************************************/

.section .vectors
    .align  2
    .globl  __Vectors
__Vectors:
                .long     g_top_irqstack
                .long     Reset_Handler
                .long     Default_Handler                      
                .long     Default_Handler                
                .long     Default_Handler                
                .long     Default_Handler                 
                .long     Default_Handler               
                .long     0                                
                .long     0                                
                .long     0                                
                .long     0                                
                .long     Default_Handler                      
                .long     Default_Handler                 
                .long     0                                
                .long     PendSV_Handler                   
                .long     SysTick_Handler                  

    .text
    .align   2
_start:
    .align   2
    .globl   Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    movs    r0, #0x0
    msr     control, r0

    ldr     r0, =g_top_irqstack
    mov     sp, r0

#ifndef __NO_SYSTEM_INIT
    ldr     r0, =SystemInit
    blx     r0
#endif

    ldr     r0, =pre_main
    blx     r0

    .size   Reset_Handler, . - Reset_Handler
__exit:
    b       __exit

PendSV_Handler:
    bkpt    #0
    b       .

.section .stack

    .align  2
    .global g_top_irqstack
    .global g_base_irqstack
g_base_irqstack:
    .space  CONFIG_ARCH_INTERRUPTSTACK
g_top_irqstack:


/*
NMI_Handler     PROC
                EXPORT  NMI_Handler                      [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler                [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler                [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler                 [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler               [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                      [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler                  [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                    [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                   [WEAK]
                B       .
                ENDP                                     
                                                          
Default_Handler PROC                                      

                EXPORT  WWDG_IRQHandler                   [WEAK]                                       
                EXPORT  PVD_AVD_IRQHandler                [WEAK]                         
                EXPORT  TAMP_STAMP_IRQHandler             [WEAK]   
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]             
                EXPORT  FLASH_IRQHandler                  [WEAK]                                        
                EXPORT  RCC_IRQHandler                    [WEAK]                                          
                EXPORT  EXTI0_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI1_IRQHandler                  [WEAK]                                           
                EXPORT  EXTI2_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI3_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI4_IRQHandler                  [WEAK]
                EXPORT  DMA1_Stream0_IRQHandler           [WEAK] 
                EXPORT  DMA1_Stream1_IRQHandler           [WEAK]                        
                EXPORT  DMA1_Stream2_IRQHandler           [WEAK]                     
                EXPORT  DMA1_Stream3_IRQHandler           [WEAK]                    
                EXPORT  DMA1_Stream4_IRQHandler           [WEAK]                        
                EXPORT  DMA1_Stream5_IRQHandler           [WEAK]                          
                EXPORT  DMA1_Stream6_IRQHandler           [WEAK]
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]
                EXPORT  ADC_IRQHandler                    [WEAK]                          
                EXPORT  FDCAN1_IT0_IRQHandler             [WEAK]                                            
                EXPORT  FDCAN2_IT0_IRQHandler             [WEAK] 
                EXPORT  FDCAN1_IT1_IRQHandler             [WEAK]                                            
                EXPORT  FDCAN2_IT1_IRQHandler             [WEAK]   
                EXPORT  EXTI9_5_IRQHandler                [WEAK]                                    
                EXPORT  TIM1_BRK_IRQHandler               [WEAK]                  
                EXPORT  TIM1_UP_IRQHandler                [WEAK]                
                EXPORT  TIM1_TRG_COM_IRQHandler           [WEAK] 
                EXPORT  TIM1_CC_IRQHandler                [WEAK]                                   
                EXPORT  TIM2_IRQHandler                   [WEAK]                                            
                EXPORT  TIM3_IRQHandler                   [WEAK]                                            
                EXPORT  TIM4_IRQHandler                   [WEAK]                                            
                EXPORT  I2C1_EV_IRQHandler                [WEAK]                                             
                EXPORT  I2C1_ER_IRQHandler                [WEAK]                                             
                EXPORT  I2C2_EV_IRQHandler                [WEAK]                                            
                EXPORT  I2C2_ER_IRQHandler                [WEAK]                                               
                EXPORT  SPI1_IRQHandler                   [WEAK]                                           
                EXPORT  SPI2_IRQHandler                   [WEAK]                                            
                EXPORT  USART1_IRQHandler                 [WEAK]                                          
                EXPORT  USART2_IRQHandler                 [WEAK]                                          
                EXPORT  USART3_IRQHandler                 [WEAK]                                         
                EXPORT  EXTI15_10_IRQHandler              [WEAK]                                  
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]                                       
                EXPORT  TIM8_BRK_TIM12_IRQHandler         [WEAK]                 
                EXPORT  TIM8_UP_TIM13_IRQHandler          [WEAK]                 
                EXPORT  TIM8_TRG_COM_TIM14_IRQHandler     [WEAK] 
                EXPORT  TIM8_CC_IRQHandler                [WEAK]                                   
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]                                          
                EXPORT  FMC_IRQHandler                    [WEAK]                                             
                EXPORT  SDMMC1_IRQHandler                 [WEAK]                                             
                EXPORT  TIM5_IRQHandler                   [WEAK]                                             
                EXPORT  SPI3_IRQHandler                   [WEAK]                                             
                EXPORT  UART4_IRQHandler                  [WEAK]                                            
                EXPORT  UART5_IRQHandler                  [WEAK]                                            
                EXPORT  TIM6_DAC_IRQHandler               [WEAK]                   
                EXPORT  TIM7_IRQHandler                   [WEAK]                    
                EXPORT  DMA2_Stream0_IRQHandler           [WEAK]                                  
                EXPORT  DMA2_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream2_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream3_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream4_IRQHandler           [WEAK]                                 
                EXPORT  ETH_IRQHandler                    [WEAK]                                         
                EXPORT  ETH_WKUP_IRQHandler               [WEAK]                     
                EXPORT  FDCAN_CAL_IRQHandler              [WEAK]                                                                                                                                                                              
                EXPORT  DMA2_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream7_IRQHandler           [WEAK]                                   
                EXPORT  USART6_IRQHandler                 [WEAK]                                           
                EXPORT  I2C3_EV_IRQHandler                [WEAK]                                              
                EXPORT  I2C3_ER_IRQHandler                [WEAK]                                              
                EXPORT  OTG_HS_EP1_OUT_IRQHandler         [WEAK]                      
                EXPORT  OTG_HS_EP1_IN_IRQHandler          [WEAK]                      
                EXPORT  OTG_HS_WKUP_IRQHandler            [WEAK]                        
                EXPORT  OTG_HS_IRQHandler                 [WEAK]                                      
                EXPORT  DCMI_IRQHandler                   [WEAK]                                             
                EXPORT  RNG_IRQHandler                    [WEAK]
                EXPORT  FPU_IRQHandler                    [WEAK]
                EXPORT  UART7_IRQHandler                  [WEAK]
                EXPORT  UART8_IRQHandler                  [WEAK]
                EXPORT  SPI4_IRQHandler                   [WEAK]
                EXPORT  SPI5_IRQHandler                   [WEAK]
                EXPORT  SPI6_IRQHandler                   [WEAK]
                EXPORT  SAI1_IRQHandler                   [WEAK]
                EXPORT  LTDC_IRQHandler                   [WEAK]
                EXPORT  LTDC_ER_IRQHandler                [WEAK]
                EXPORT  DMA2D_IRQHandler                  [WEAK]
                EXPORT  SAI2_IRQHandler                   [WEAK]   
                EXPORT  QUADSPI_IRQHandler                [WEAK]
                EXPORT  LPTIM1_IRQHandler                 [WEAK]
                EXPORT  CEC_IRQHandler                    [WEAK]   
                EXPORT  I2C4_EV_IRQHandler                [WEAK]
                EXPORT  I2C4_ER_IRQHandler                [WEAK] 
                EXPORT  SPDIF_RX_IRQHandler               [WEAK]
                EXPORT  OTG_FS_EP1_OUT_IRQHandler         [WEAK]
                EXPORT  OTG_FS_EP1_IN_IRQHandler          [WEAK]                      
                EXPORT  OTG_FS_WKUP_IRQHandler            [WEAK]                      
                EXPORT  OTG_FS_IRQHandler                 [WEAK]                        
                EXPORT  DMAMUX1_OVR_IRQHandler            [WEAK]
                EXPORT  HRTIM1_Master_IRQHandler          [WEAK] 
                EXPORT  HRTIM1_TIMA_IRQHandler            [WEAK]                                      
                EXPORT  HRTIM1_TIMB_IRQHandler            [WEAK]                                      
                EXPORT  HRTIM1_TIMC_IRQHandler            [WEAK]                                      
                EXPORT  HRTIM1_TIMD_IRQHandler            [WEAK]                                      
                EXPORT  HRTIM1_TIME_IRQHandler            [WEAK]                                      
                EXPORT  HRTIM1_FLT_IRQHandler             [WEAK]
                EXPORT  DFSDM1_FLT0_IRQHandler            [WEAK] 
                EXPORT  DFSDM1_FLT1_IRQHandler            [WEAK]                                             
                EXPORT  DFSDM1_FLT2_IRQHandler            [WEAK]                                             
                EXPORT  DFSDM1_FLT3_IRQHandler            [WEAK]                                                                                        
                EXPORT  SAI3_IRQHandler                   [WEAK]                                             
                EXPORT  SWPMI1_IRQHandler                 [WEAK]                            
                EXPORT  TIM15_IRQHandler                  [WEAK]                                             
                EXPORT  TIM16_IRQHandler                  [WEAK]                                              
                EXPORT  TIM17_IRQHandler                  [WEAK]                                            
                EXPORT  MDIOS_WKUP_IRQHandler             [WEAK]                                             
                EXPORT  MDIOS_IRQHandler                  [WEAK]                                              
                EXPORT  JPEG_IRQHandler                   [WEAK]                                               
                EXPORT  MDMA_IRQHandler                   [WEAK]                                                                                             
                EXPORT  SDMMC2_IRQHandler                 [WEAK]                                             
                EXPORT  HSEM1_IRQHandler                  [WEAK]                                                                                                             
                EXPORT  ADC3_IRQHandler                   [WEAK]                                                
                EXPORT  DMAMUX2_OVR_IRQHandler            [WEAK]                                            
                EXPORT  BDMA_Channel0_IRQHandler          [WEAK]                                      
                EXPORT  BDMA_Channel1_IRQHandler          [WEAK]                                      
                EXPORT  BDMA_Channel2_IRQHandler          [WEAK]                                      
                EXPORT  BDMA_Channel3_IRQHandler          [WEAK]                                      
                EXPORT  BDMA_Channel4_IRQHandler          [WEAK]                                     
                EXPORT  BDMA_Channel5_IRQHandler          [WEAK]                                      
                EXPORT  BDMA_Channel6_IRQHandler          [WEAK]                                      
                EXPORT  BDMA_Channel7_IRQHandler          [WEAK]                                     
                EXPORT  COMP1_IRQHandler                  [WEAK]                                              
                EXPORT  LPTIM2_IRQHandler                 [WEAK]                                           
                EXPORT  LPTIM3_IRQHandler                 [WEAK]                                            
                EXPORT  LPTIM4_IRQHandler                 [WEAK]                                            
                EXPORT  LPTIM5_IRQHandler                 [WEAK]                                            
                EXPORT  LPUART1_IRQHandler                [WEAK]                                                  
                EXPORT  CRS_IRQHandler                    [WEAK]
                EXPORT  ECC_IRQHandler                    [WEAK] 				
                EXPORT  SAI4_IRQHandler                   [WEAK]                                                                                     
                EXPORT  WAKEUP_PIN_IRQHandler             [WEAK] 


WWDG_IRQHandler                                                          
PVD_AVD_IRQHandler                                             
TAMP_STAMP_IRQHandler                
RTC_WKUP_IRQHandler                            
FLASH_IRQHandler                                                          
RCC_IRQHandler                                                              
EXTI0_IRQHandler                                                              
EXTI1_IRQHandler                                                             
EXTI2_IRQHandler                                                              
EXTI3_IRQHandler                                                              
EXTI4_IRQHandler 
DMA1_Stream0_IRQHandler
DMA1_Stream1_IRQHandler                                  
DMA1_Stream2_IRQHandler                               
DMA1_Stream3_IRQHandler                              
DMA1_Stream4_IRQHandler                                  
DMA1_Stream5_IRQHandler                                    
DMA1_Stream6_IRQHandler                   
ADC_IRQHandler                                           
FDCAN1_IT0_IRQHandler
FDCAN2_IT0_IRQHandler
FDCAN1_IT1_IRQHandler
FDCAN2_IT1_IRQHandler
EXTI9_5_IRQHandler                                                
TIM1_BRK_IRQHandler                        
TIM1_UP_IRQHandler                      
TIM1_TRG_COM_IRQHandler  
TIM1_CC_IRQHandler                                               
TIM2_IRQHandler                                                           
TIM3_IRQHandler                                                           
TIM4_IRQHandler                                                           
I2C1_EV_IRQHandler                                                         
I2C1_ER_IRQHandler                                                         
I2C2_EV_IRQHandler                                                        
I2C2_ER_IRQHandler                                                           
SPI1_IRQHandler                                                          
SPI2_IRQHandler                                                           
USART1_IRQHandler                                                       
USART2_IRQHandler                                                       
USART3_IRQHandler                                                      
EXTI15_10_IRQHandler                                            
RTC_Alarm_IRQHandler                                                           
TIM8_BRK_TIM12_IRQHandler                      
TIM8_UP_TIM13_IRQHandler                       
TIM8_TRG_COM_TIM14_IRQHandler  
TIM8_CC_IRQHandler                                               
DMA1_Stream7_IRQHandler                                                 
FMC_IRQHandler                                                            
SDMMC1_IRQHandler                                                            
TIM5_IRQHandler                                                            
SPI3_IRQHandler                                                            
UART4_IRQHandler                                                          
UART5_IRQHandler                                                          
TIM6_DAC_IRQHandler                            
TIM7_IRQHandler                              
DMA2_Stream0_IRQHandler                                         
DMA2_Stream1_IRQHandler                                          
DMA2_Stream2_IRQHandler                                           
DMA2_Stream3_IRQHandler                                           
DMA2_Stream4_IRQHandler                                        
ETH_IRQHandler                                                         
ETH_WKUP_IRQHandler                                
FDCAN_CAL_IRQHandler                                                                                                                                                                                                                            
DMA2_Stream5_IRQHandler                                          
DMA2_Stream6_IRQHandler                                          
DMA2_Stream7_IRQHandler                                          
USART6_IRQHandler                                                        
I2C3_EV_IRQHandler                                                          
I2C3_ER_IRQHandler                                                          
OTG_HS_EP1_OUT_IRQHandler 
OTG_HS_EP1_IN_IRQHandler 
OTG_HS_WKUP_IRQHandler   
OTG_HS_IRQHandler        
DCMI_IRQHandler                                                            
RNG_IRQHandler
FPU_IRQHandler  
UART7_IRQHandler                  
UART8_IRQHandler                  
SPI4_IRQHandler                   
SPI5_IRQHandler                   
SPI6_IRQHandler                   
SAI1_IRQHandler                   
LTDC_IRQHandler                   
LTDC_ER_IRQHandler                 
DMA2D_IRQHandler   
SAI2_IRQHandler        
QUADSPI_IRQHandler
LPTIM1_IRQHandler
CEC_IRQHandler
I2C4_EV_IRQHandler
I2C4_ER_IRQHandler
SPDIF_RX_IRQHandler
OTG_FS_EP1_OUT_IRQHandler
OTG_FS_EP1_IN_IRQHandler 
OTG_FS_WKUP_IRQHandler   
OTG_FS_IRQHandler        
DMAMUX1_OVR_IRQHandler
HRTIM1_Master_IRQHandler               
HRTIM1_TIMA_IRQHandler                                                      
HRTIM1_TIMB_IRQHandler                                                      
HRTIM1_TIMC_IRQHandler                                                      
HRTIM1_TIMD_IRQHandler                                                      
HRTIM1_TIME_IRQHandler                                                      
HRTIM1_FLT_IRQHandler 
DFSDM1_FLT0_IRQHandler
DFSDM1_FLT1_IRQHandler                                                                
DFSDM1_FLT2_IRQHandler                                                                
DFSDM1_FLT3_IRQHandler                                                                                                                                
SAI3_IRQHandler                                                                    
SWPMI1_IRQHandler                                                 
TIM15_IRQHandler                                                                   
TIM16_IRQHandler                                                                    
TIM17_IRQHandler                                                                  
MDIOS_WKUP_IRQHandler                                                              
MDIOS_IRQHandler                                                                    
JPEG_IRQHandler                                                                      
MDMA_IRQHandler                                                                                                                                            
SDMMC2_IRQHandler                                                                  
HSEM1_IRQHandler                                                                                                                                       
ADC3_IRQHandler                                                                       
DMAMUX2_OVR_IRQHandler                                                            
BDMA_Channel0_IRQHandler                                                     
BDMA_Channel1_IRQHandler                                                     
BDMA_Channel2_IRQHandler                                                     
BDMA_Channel3_IRQHandler                                                     
BDMA_Channel4_IRQHandler                                                    
BDMA_Channel5_IRQHandler                                                     
BDMA_Channel6_IRQHandler                                                     
BDMA_Channel7_IRQHandler                                                    
COMP1_IRQHandler                                                                    
LPTIM2_IRQHandler                                                                
LPTIM3_IRQHandler                                                                 
LPTIM4_IRQHandler                                                                 
LPTIM5_IRQHandler                                                                 
LPUART1_IRQHandler                                                                                                                         
CRS_IRQHandler
ECC_IRQHandler                                                            
SAI4_IRQHandler      
WAKEUP_PIN_IRQHandler
*/