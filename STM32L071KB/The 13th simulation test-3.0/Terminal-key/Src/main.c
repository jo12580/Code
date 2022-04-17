/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l0xx_hal.h"
#include "zm4xx_gpio.h"
#include "spi.h"
#include "timer.h"
#include "stddef.h"
#include "sx127x_lora.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define FREQ  (433000000)
void SystemClock_Config(void);
void Hardware_Init(void);
radio_handle_t  handle;
int ret = RADIO_RET_OK;

    uint16_t len = 0;
		uint8_t rx = 0;
		uint8_t tx = 0x55;
		uint8_t cntr = 0;
	unsigned int time_i=0,time_10ms=1,time_100ms=1,time_800ms=1,time_1s=1;
	unsigned int lora_flag=0;
	unsigned int oled_x1=0,oled_x2=0;
	char lcdLine_1st_line[16]="123";
  char lcdLine_2st_line[16]="123";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Task_BrdInit(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);

    OLED_PowerControl(ENABLE);

    HAL_Delay(200);
    OLED_Init();
    OLED_Clear();

    OLED_ShowString(0, 0, (unsigned char *)"                ", 16);
    OLED_ShowString(0, 2, (unsigned char *)"                ", 16);
}
void LoRa_Init(void)
{
	
	handle = radio_zm4xx_inst_init(spi_recv_byte,spi_send_byte,zm4xx_rst_pin_set,zm4xx_sel_pin_set,delay_ms,delay_us);
	
	if(handle != NULL){
		if(RADIO_RET_OK == radio_freq_set(handle, FREQ)){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_RESET);
		}
		
		radio_mode_set(handle, RX_MODE);	//Receive Mode
	}
}
void key(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
    Task_BrdInit();
		zm4xx_pin_init(); //LORA需要
    spi_init();//LORA需要
		LoRa_Init();
		HAL_TIM_Base_Start_IT(&htim6);
		HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			if(time_10ms==1)
			{
				key_scan();
				time_10ms=0;
			}
			key();
			if(time_800ms==1)
			{
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				time_800ms=0;
			}
			if(time_100ms==1)
			{
				OLED_ShowString(oled_x1, 0, (unsigned char *)lcdLine_1st_line, 16);
				OLED_ShowString(oled_x2, 2, (unsigned char *)lcdLine_2st_line, 16);
				time_100ms=0;
			}
			radio_buf_send(handle, (unsigned char *)lcdLine_2st_line, strlen(lcdLine_2st_line));
    }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN 4 */
void key()
{
	static unsigned int key_11=1,key_12=0,hour=0,min=0,sec=0;
	if(key_11==1)
	{
		sprintf(lcdLine_1st_line, "RTC");
		sprintf(lcdLine_2st_line, "%2d-%2d-%2d",sTime.Hours,sTime.Minutes,sTime.Seconds);
		oled_x1=56;
		oled_x2=32;
		if(time_1s==1)
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
			time_1s=0;
		}
	}
	if(key_11==0)
	{
		sTime.Hours=hour;
		sTime.Minutes=min;
		sTime.Seconds=sec;
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		sprintf(lcdLine_1st_line, "SET");
		sprintf(lcdLine_2st_line, "%2d-%2d-%2d",hour,min,sec);
		oled_x1=56;
		oled_x2=32;
	}
	if(key_flag!=0)
	{
		if(key_flag==11)
		{
			key_11=!key_11;
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
		}
		if(key_flag==12)
		{
			if(key_11==0)
			{
				key_12++;
				if(key_12==3)	key_12=0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
			}
		}
		if(key_flag==21)
		{
			switch (key_12)
			{
				case 0:
				{
					hour++;
					if(hour==24)	hour=0;
				}break;
				case 1:
				{
					min++;
					if(min==60)		min=0;
				}break;
				case 2:
				{
					sec++;
					if(sec==60)		sec=0;
				}break;
			}
		}
		if(key_flag==22)
		{
			radio_buf_send(handle, (unsigned char *)lcdLine_2st_line, strlen(lcdLine_2st_line));
		}
		key_flag=0;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	{
		time_i++;
		if(time_i==9999)		time_i=0;
		if(time_i%10==0)			time_10ms=!time_10ms;
		if(time_i%101==0)		time_100ms=!time_100ms;
		if(time_i%801==0)		time_800ms=!time_800ms;
		if(time_i%1000==0)	time_1s=!time_1s;
	}
}
/* USER CODE END 4 */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
