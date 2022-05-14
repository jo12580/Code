/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "oled.h"
#include "ds18b20.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char lcdLine_1st_line[16];
char lcdLine_2st_line[16];
unsigned int time_100ms=1,time_500ms=1;
unsigned int key_flag=0;
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
void key(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	short value;
	float value1;
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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(100);
	OLED_Init();
	HAL_TIM_Base_Start_IT(&htim1);
//	sprintf(lcdLine_2st_line, "welcome");
	DS18B20_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(time_500ms==1)
		{
//			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//			HAL_RTC_GetDate(&hrtc, &sdata, RTC_FORMAT_BIN);
//			value=DS18B20_Get_Temp();
//			sprintf(lcdLine_2st_line, "%.3f",value);
			time_500ms=0;
		}
		if(time_100ms==1)
		{
			value1=DS18B20_Get_Temp();
			sprintf(lcdLine_2st_line, "%.2f",value1);
			OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
			OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
			time_100ms=0;
		}
//		key();
  }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN 4 */
void key()
{
	static unsigned int hour,min,sec;
	static unsigned int time_allow=0,clock_allow=0;
	static unsigned int key_1=0;	//判断按键1按下次数
	if(key_flag!=0)		
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
		if(key_flag==1)		//设置时间或闹钟的按键
		{
			if(key_1==1)		//设置闹钟
			{
				time_allow=0;clock_allow=1;
				OLED_ShowString(84, 2, (unsigned char *)"alarm ", 8);
			}
			if(key_1==0)		//设置时间
			{
				hour=sTime.Hours;
				min=sTime.Minutes;
				sec=sTime.Seconds;
				time_allow=1;clock_allow=0;
				OLED_ShowString(84, 2, (unsigned char *)"time", 8);
			}
			key_1++;
			if(key_1==2)	key_1=0;
		}
		if(key_flag==2)		//确认键
		{
			if(time_allow==1)
			{
				sTime.Hours=hour;
				sTime.Minutes=min;
				sTime.Seconds=sec;
				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			}
		}
		if(key_flag==6)		//断电键
		{
			time_allow=0;clock_allow=0;
			sprintf(lcdLine_2st_line, "                   ");
		}
			
			//以下代码，附属摁键1之后的操作
			if(time_allow==1)
			{
				if(key_flag==3)
				{
					hour++;
					if(hour==24)	hour=0;
				}
				if(key_flag==4)
				{
					min++;
					if(min==60)		min=0;
				}
				if(key_flag==5)
				{
					sec++;
					if(sec==60)		sec=0;
				}
				sprintf(lcdLine_2st_line, "%2d:%2d:%2d",hour,min,sec);
			}
		key_flag=0;
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static unsigned int i=0;
	if(GPIO_Pin==GPIO_PIN_5)
	{
		i++;
		if(i==2)
		{
			key_flag=1;		//设置键
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_6)
	{
		i++;
		if(i==2)
		{
			key_flag=2;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_7)
	{
		i++;
		if(i==3)
		{
			key_flag=3;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_8)
	{
		i++;
		if(i==2)
		{
			key_flag=4;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_9)
	{
		i++;
		if(i==2)
		{
			key_flag=5;
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_10)
	{
		i++;
		if(i==2)
		{
			key_flag=6;
			HAL_NVIC_SystemReset();
			i=0;
		}
	}
	if(GPIO_Pin==GPIO_PIN_11)
	{
		i++;
		if(i==2)
		{
			key_flag=7;
			i=0;
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	if(htim->Instance==TIM1)
	{
		static unsigned int a=0,b=0;
		a++;b++;
		if(a==100)
		{
			time_100ms=!time_100ms;
			a=0;
		}
		if(b==500)
		{
			time_500ms=!time_500ms;
			b=0;
		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
