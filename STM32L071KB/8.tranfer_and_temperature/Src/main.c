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
#include "gpio.h"
#include "stm32l0xx_hal.h"
#include "zm4xx_gpio.h"
#include "spi.h"
#include "timer.h"
#include "stddef.h"
#include "sx127x_lora.h"
#include "temperature.h"
#include "oled.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	#define FREQ  (433000000)
	void Hardware_Init(void);
	radio_handle_t  handle;
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
//    char lcdLine_1st_line[16];
    char lcdLine_2st_line[16];
//		int ret = RADIO_RET_OK;

    uint16_t len = 0;
		uint8_t rx = 0;
		uint8_t tx = 0x55;
		uint8_t cntr = 0;
		float a;
		int key;


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
	Task_BrdInit();
	LoRa_Init();
	zm4xx_pin_init(); //LORA需要
	spi_init();//LORA需要
	LoRa_Init();
  /* USER CODE BEGIN 2 */
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
			a=Get_Temperature();
			sprintf(lcdLine_2st_line, "%f  ",a);
			OLED_ShowString(0, 2, (uint8_t *)lcdLine_2st_line, 16);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)==0)
			{
				HAL_Delay(50);
				key++;
//					HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
//					while(key%2==0)
					{			
						a=Get_Temperature();
						sprintf(lcdLine_2st_line, "%f  ",a);
						OLED_ShowString(0, 2, (uint8_t *)lcdLine_2st_line, 16);
						radio_buf_send(handle,(uint8_t *)lcdLine_2st_line , 8); 	//
						radio_mode_set(handle, RX_MODE);	
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
						if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)==0)
						{
							key=2;
						}
				}
			}
			
//			
//			
//			
//			
//			
//			ret = radio_buf_recv(handle, &rx, &len);
//			if(ret == RADIO_RET_OK){
//				ret = -1;
//				if(rx == 0x55){
//						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
//				}
//			}
//			
//			if(++cntr == 40){
//				radio_buf_send(handle, &tx, 1); 	//
//				radio_mode_set(handle, RX_MODE);	
//				cntr = 0;
//			}
//		  HAL_Delay(2);


			
//		sprintf(lcdLine_1st_line, "GXCT IOT       ");
//		sprintf(lcdLine_2st_line, "Hello World    ");

//		OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
//		OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
    }
		
		
  /* USER CODE END 3 */
}

















/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
