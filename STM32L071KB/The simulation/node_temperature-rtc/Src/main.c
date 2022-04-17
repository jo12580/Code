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
#include "i2c_2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define FREQ  (433000000)
void SystemClock_Config(void);
void Hardware_Init(void);
radio_handle_t  handle;
 int ret = RADIO_RET_OK;

    uint16_t len = 0;
		uint8_t rx[50];
		uint8_t tx = 0x55;
		uint8_t cntr = 0;
	unsigned int time_100ms=1,time_500ms=1;
	unsigned int lora_flag=2,send_allow=1;
	float value;
	unsigned char cmd1[]="0x0A,0x0B,0xB1,0xEE,0xEF";
	unsigned char cmd2[]="0x0A,0x0B,0xB2,0xEE,0xEF";
	unsigned char cmd3[]="0x0A,0x0B,0xB3,0xEE,0xEF";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//	ret = radio_buf_recv(handle, &rx, &len);
//			if(ret == RADIO_RET_OK){
//				if(rx == 0xAA){
//						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
//				}
//			}
//			
//			if(++cntr == 40){
//				radio_buf_send(handle, &tx, 1); 	//
//				radio_mode_set(handle, RX_MODE);	
//				cntr = 0;
//			}
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
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_SET);
		}
		
		radio_mode_set(handle, RX_MODE);	//Receive Mode
	}
}
void node(void);
unsigned int defferen(unsigned char *p);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    char lcdLine_1st_line[16];
    char lcdLine_2st_line[16];


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
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
    Task_BrdInit();
		zm4xx_pin_init(); //LORA需要
    spi_init();//LORA需要
		LoRa_Init();
		HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			if(time_500ms==1)
			{
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				sprintf(lcdLine_1st_line,  "%2d-%2d-%d",sTime.Hours,sTime.Minutes,sTime.Seconds);
				sprintf(lcdLine_2st_line,"%.2f C",value);
				value=temperature();
				time_500ms=0;
			}
			if(time_100ms==1)
			{
				OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
				OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
				time_100ms=0;
			}
			node();
		}
  /* USER CODE END 3 */
}
/* USER CODE BEGIN 4 */
void node()
{
	unsigned int i=0;
	static unsigned int led=0;
	char send[20];
	if(lora_flag==1)
	{
		lora_flag=0;
		send_allow=0;
		i=defferen(rx);
		if(i==1)
		{
			sprintf(send,"CmdB1ok");
			radio_buf_send(handle, (unsigned char *)send, strlen(send)); 
			radio_mode_set(handle, RX_MODE);
		}
		if(i==2)
		{
			sprintf(send,"CmdB2ok,%2d-%2d-%d,%.2fC",sTime.Hours,sTime.Minutes,sTime.Seconds,value);
			radio_buf_send(handle, (unsigned char *)send, strlen(send)); 
			radio_mode_set(handle, RX_MODE);
		}
		if(i==3)
		{
			sprintf(send,"CmdB3ok");
			radio_buf_send(handle, (unsigned char *)send, strlen(send)); 
			radio_mode_set(handle, RX_MODE);
			time_100ms=2;
			led=1;
		}
		send_allow=1;
	}
	if(led==1)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
		if(time_100ms==0)
		{
			led=0;
		}
	}
	if(led==0)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
}
unsigned int defferen(unsigned char *p)
{
	unsigned int i=0,num1=0,num2=0,num3=0;
	while (p[i] != '\0')
	{
		i++;
		if(p[i]==cmd1[i])
		{
			num1++;
		}
		if(p[i]==cmd2[i])
		{
			num2++;
		}
		if(p[i]==cmd3[i])
		{
			num3++;
		}
	}
	if(num1==5)	
		return 1;
	else if(num2==5)	
		return 2;
	else if(num3==5)	
		return 2;
	else 
		return 0;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static unsigned int a1=0,a2=0;
	a1++;
	a2++;
	if(a1==100)	
	{
		time_100ms=!time_100ms;
		a1=0;
	}
	if(a2==500)
	{
		time_500ms=!time_500ms;
		a2=0;
	}		
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_10)
	{
		if(send_allow==1)
		{
			ret = radio_buf_recv(handle, rx, &len);
			lora_flag=1;
		}
	}
	if(GPIO_Pin==GPIO_PIN_14)
	{
		
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
