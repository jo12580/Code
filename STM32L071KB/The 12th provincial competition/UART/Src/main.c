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
#include "adc.h"
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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define FREQ  (433000000)
void SystemClock_Config(void);
void Hardware_Init(void);
radio_handle_t  handle;
int ret = RADIO_RET_OK;

    uint16_t len = 0;
		uint8_t rx[100];
		uint8_t tx = 0x55;
		uint8_t cntr = 0;
	unsigned int time_i=0,time_100ms=1;
	unsigned char send[50]="123";
	unsigned char reply[50];
	unsigned char vrp1[10]="VRP1?",vrp2[10]="VRP2?";
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
void rx2(void);
void clear(unsigned char *send);
unsigned int difference(void);
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
	unsigned int i;
	unsigned char a[]="\r\n";

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
  MX_ADC_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
    Task_BrdInit();
		zm4xx_pin_init(); //LORA需要
    spi_init();//LORA需要
		LoRa_Init();
		HAL_TIM_Base_Start_IT(&htim6);
		HAL_TIM_Base_Start_IT(&htim7);
		
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
		HAL_UART_Receive_DMA(&huart2,reply,20);
			sprintf(lcdLine_1st_line, "GXCT IOT       ");
			sprintf(lcdLine_2st_line, "Hello World    ");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			if(difference()==1)
			{
				radio_buf_send(handle,vrp1,sizeof(vrp1));
				radio_buf_send(handle,vrp1,sizeof(vrp1));
				radio_mode_set(handle, RX_MODE);	
				ret = radio_buf_recv(handle, rx, &len);
				i=5000;
				while(i--&&ret != RADIO_RET_OK)
					ret = radio_buf_recv(handle, rx, &len);
				if(ret == RADIO_RET_OK)
				{
//					HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
					HAL_UART_Transmit(&huart2,rx,sizeof(rx),100);
					HAL_UART_Transmit(&huart2,a,sizeof(a),100);
				}
			}
			else if(difference()==2)
			{
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
				radio_buf_send(handle,vrp2,sizeof(vrp2));
				radio_buf_send(handle,vrp2,sizeof(vrp2));
				radio_mode_set(handle, RX_MODE);	
				ret = radio_buf_recv(handle, rx, &len);
				i=5000;
				while(i--&&ret != RADIO_RET_OK)
					ret = radio_buf_recv(handle, rx, &len);
				if(ret == RADIO_RET_OK)
				{
					HAL_UART_Transmit(&huart2,rx,sizeof(rx),100);
					HAL_UART_Transmit(&huart2,a,sizeof(a),100);
				}
			}
//			else if(difference()==0)
//			{
//				radio_buf_send(handle,a,sizeof(a));
//			}
			
			
			if(time_100ms==1)
			{
				OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
				OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
			}
    }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN 4 */
unsigned int difference()
{
	unsigned int i,num1,num2,flag;
	for(i=0;i<=4;i++)
	{
		if(reply[i]==vrp1[i])
			num1++;
	}
	for(i=0;i<=4;i++)
	{
		if(reply[i]==vrp2[i])
			num2++;
	}
	if(num1==5)				flag=1;
	else if(num2==5)	flag=2;
	else flag=0;
//	clear(reply);
	return flag;
}
void rx2()
{
	unsigned char a[]="\r\n";
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)==SET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		HAL_UART_DMAStop(&huart2);
		HAL_UART_Transmit(&huart2,reply,sizeof(reply),100);
		HAL_UART_Transmit(&huart2,a,sizeof(a),100);
//		clear(reply);
		HAL_UART_Receive_DMA(&huart2,reply,20);
	}
}
void clear(unsigned char *send)
{
	unsigned int i;
	for(i=0;i<=50;i++)
	{
		send[i]=0;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	{
		time_i++;
		if(time_i==9999)	time_i=0;
		if(time_i%100==0)
		{
			time_100ms=!time_100ms;
		}
	}
}
//ret = radio_buf_recv(handle, &rx, &len);
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
