#include "stm32l0xx_hal.h"
#include "zm4xx_gpio.h"
#include "oled.h"
#include "led.h"
#include "relay.h"
#include "spi.h"
#include "timer.h"
#include "button.h"
#include "stddef.h"
#include "stdio.h"
#include "sx127x_lora.h"
#include "temperature.h"


void SystemClock_Config(void);
void Hardware_Init(void);
uint8_t        __g_data_buf[256] = {0};
uint16_t       __g_recv_byte     = 0;
radio_handle_t __gp_handle;
char *__gp_test_str;
static uint16_t __g_send_byte     = 0;
#define FREQ  (433000000)
uint8_t key_flag = 0; //按键标志位



int main(void)
{
    radio_handle_t  handle;
    uint16_t i   = 0;
    uint16_t len = 0;
    uint16_t  j = 0;
    int ret = RADIO_RET_OK;
    uint8_t key_num = 0;
    Hardware_Init();
		float *a;
	char lcdLine_2st_line[16];
    while(1)
    {
        handle = radio_zm4xx_inst_init(spi_recv_byte, spi_send_byte, zm4xx_rst_pin_set, zm4xx_sel_pin_set, delay_ms, delay_us); //LORA初始化
				
			
        if (handle != NULL)
        {
            /* 设置频率 */
            if(RADIO_RET_OK == radio_freq_set(handle, FREQ))
            {
                radio_mode_set(handle, RX_MODE);//进入接收模式
            }
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
        }
        while(1)
        {
//            key_num = BUTTON_Scan();
//            if(key_num == 1) //按键发送数据
//            {
//                key_flag++;
//                if(key_flag == 4)key_flag = 1;
//                switch(key_flag)
//                {
//                case 1:
//                {
//                    __gp_test_str = "a"; //第一位为设备号，后面8位为学号，最后三位为LED1、LED2、传感器测量信号，为1为开或者测量数据
//                    OLED_ShowString(i * 16, 0, (uint8_t *)"  transmitting  ", 16);
//                    OLED_ShowString(i * 16, 2, (uint8_t *)"      1100      ", 16);
//                    break;
//                }
//                case 2:
//                {
//                    __gp_test_str = "1010"; //第一位为设备号，后面8位为学号，最后三位为LED1、LED2、传感器测量信号，为1为开或者测量数?
//                    OLED_ShowString(i * 16, 0, (uint8_t *)"  transmitting  ", 16);
//                    OLED_ShowString(i * 16, 2, (uint8_t *)"      1010      ", 16);
//                    break;
//                }
//                case 3:
//                {
//                    __gp_test_str = "1000"; //第一位为设备号，后面8位为学号，最后三位为LED1、LED2、传感器测量信号，为1为开或者测量数据
//                    OLED_ShowString(i * 16, 0, (uint8_t *)"  transmitting  ", 16);
//                    OLED_ShowString(i * 16, 2, (uint8_t *)"      1000      ", 16);
//                    break;
//                }
//                }
								*a=Get_Temperature();
//							sprintf(lcdLine_2st_line, "%.2f  ", ret);
            //  OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
                for (j = 0; j < 8; j++)
                {
                    __g_data_buf[j] = a[j];
                }
//								__g_data_buf[0]=a;
                __g_send_byte  = 8;
                radio_buf_send(handle, __g_data_buf, __g_send_byte);
                radio_mode_set(handle, RX_MODE);
//                key_num = 0;
//            }

            /*  轮询接收  */
            ret = radio_buf_recv(handle, __g_data_buf, &len);  /*  接收数据  */
            if (ret == RADIO_RET_OK)
            {
                __g_recv_byte = len;
            }
            // sprintf(lcdLine_2st_line, "%.2f  ", ret);
            // OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
						
						
           if(__g_recv_byte > 0)
           {
						 sprintf(lcdLine_2st_line, "%.2f  ",(__g_data_buf));
						 OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
//               if((*__g_data_buf) == '1') //从机
//               {
//                   if(*(__g_data_buf + 1) == '1') //打开LED1
//                   {
//                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//                       OLED_ShowString(0, 0, (uint8_t *)"NO.1    LED1 ON ", 16);
//                   }
//                   else
//                   {
//                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//                       OLED_ShowString(0, 0, (uint8_t *)"NO.1    LED1 OFF", 16);
//                   }
//                   if(*(__g_data_buf + 2) == '1') //打开LED2
//                   {
//                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
//                       OLED_ShowString(i * 16, 2, (uint8_t *)"NO.1    LED2 ON ", 16);
//                   }
//                   else
//                   {
//                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
//                       OLED_ShowString(i * 16, 2, (uint8_t *)"NO.1    LED2 OFF", 16);
//                   }
//               }
               radio_mode_set(handle, RX_MODE);
               __g_recv_byte = 0;
           }
        }
    }
}

void Hardware_Init(void)
{
    SystemClock_Config();
    HAL_Init();
    zm4xx_pin_init(); //LORA需要
    spi_init();//LORA需要
    Button_Init();
    OLED_Init();
    OLED_Clear();
    RELAY_Init();
}



void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable MSI Oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    RCC_OscInitStruct.MSICalibrationValue = 0x00;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }

    /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /* Disable Power Control clock */
    __HAL_RCC_PWR_CLK_DISABLE();

}

