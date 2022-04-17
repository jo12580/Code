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
#include "temperature.h"
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

unsigned int key=0;
float max=26.0,min=22.0;
unsigned char send[7],rec[7];
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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    char lcdLine_1st_line[20];
    char lcdLine_2st_line[20];
		char lcdLine_3st_line[20];
	float tem;
	unsigned int i,num,k1,k2;

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
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
    Task_BrdInit();
		zm4xx_pin_init(); //LORA需要
    spi_init();//LORA需要
		LoRa_Init();
		HAL_UART_Receive_DMA(&huart2,rec,sizeof(rec));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			tem=Get_Temperature();
			if(key==0)
			{
				sprintf(lcdLine_1st_line, "			Temperature					");
				sprintf(lcdLine_2st_line, "					 %.1f 				  ",tem);

				OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
				OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
			}
			if(key==1)
			{
				sprintf(lcdLine_1st_line, "     Tmax: %.1f          ",max);
				sprintf(lcdLine_2st_line, "     Tmin: %.1f          ",min);

				OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
				OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
			}
			
			if(tem>max)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
				k1=0;
				k2=1;
			}
			else if(tem<min)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				k1=1;
				k2=0;
			}
			else 
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				k1=0;
				k2=0;
			}
			
			radio_mode_set(handle, RX_MODE);
			ret = radio_buf_recv(handle, rx, &len);
			i=5000;
			while(i--&&ret != RADIO_RET_OK)
				ret = radio_buf_recv(handle, rx, &len);
			if(ret == RADIO_RET_OK)
			{
				sscanf((char *)rx,"%d",&num);
				switch (num)
				{
					case 11:
					{
						sprintf(lcdLine_3st_line, "Temperatur %2.1f",tem);
						radio_buf_send(handle, (unsigned char *)lcdLine_3st_line, sizeof(lcdLine_3st_line)); 
						radio_buf_send(handle, (unsigned char *)lcdLine_3st_line, sizeof(lcdLine_3st_line)); 
					}break;
					case 12:
					{
						sprintf(lcdLine_3st_line, "Tmax:%2.1f Tmin:%2.1f",max,min);
						radio_buf_send(handle, (unsigned char *)lcdLine_3st_line, sizeof(lcdLine_3st_line)); 
						radio_buf_send(handle, (unsigned char *)lcdLine_3st_line, sizeof(lcdLine_3st_line)); 
					}break;
					case 21:
					{
						sprintf(lcdLine_1st_line, "K1_Status %d",k1);
						radio_buf_send(handle, (unsigned char *)lcdLine_1st_line, sizeof(lcdLine_1st_line)); 
						radio_buf_send(handle, (unsigned char *)lcdLine_1st_line, sizeof(lcdLine_1st_line)); 
					}break;
					case 22:
					{
						sprintf(lcdLine_1st_line, "K2_Status %d",k2);
						radio_buf_send(handle, (unsigned char *)lcdLine_1st_line, sizeof(lcdLine_1st_line));
						radio_buf_send(handle, (unsigned char *)lcdLine_1st_line, sizeof(lcdLine_1st_line)); 						
					}break;
				}
			}
    }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	float max1=0,min1=0;
	if(rec[0]=='M'&&rec[1]=='A'&&rec[2]=='X')
	{
		sscanf((char *)rec,"MAX%f",&max1);
		if(max1>min)
		{
			max=max1;
			HAL_UART_Transmit(&huart2,(uint8_t *)"OK\n",3,100);
		}
		else
		{
			HAL_UART_Transmit(&huart2,(uint8_t *)"ERROR\n",6,100);
		}
	}
	else if(rec[0]=='M'&&rec[1]=='I'&&rec[2]=='N')
	{
		sscanf((char *)rec,"MIN%f",&min1);
		if(min1<max)
		{
			min=min1;
			HAL_UART_Transmit(&huart2,(uint8_t *)"OK\n",3,100);
		}
		else
		{
			HAL_UART_Transmit(&huart2,(uint8_t *)"ERROR\n",6,100);
		}
	}
	else
	{
		HAL_UART_Transmit(&huart2,(uint8_t *)"ERROR\n",6,100);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	key++;
	if(key==2) key=0;
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
