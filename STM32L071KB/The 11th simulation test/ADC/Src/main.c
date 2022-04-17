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
#include "rtc.h"
#include "tim.h"
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
		uint8_t rx[50];
		uint8_t tx = 0x55;
		uint8_t cntr = 0;
unsigned char cmd_send[10]={0x14,0x02,0x35,0x69,0x41};
unsigned char cmd_send1[10]={0x14,0x02,0x35,0x69,0x41,0x0B,0x11};
unsigned char cmd_send2[10]={0x14,0x02,0x35,0x69,0x41,0x0B,0x12};
unsigned char cmd_send3[10]={0x14,0x02,0x35,0x69,0x41,0x0B,0x12};
unsigned char cmd_reply[10];
unsigned int time_i=0,time_10ms=1,time_100ms=1,time_2s=20;
float value[2];
		
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
unsigned int check(void);
void led(void);
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
  MX_CRC_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
    Task_BrdInit();
		zm4xx_pin_init(); //LORA需要
    spi_init();//LORA需要
		LoRa_Init();
		HAL_TIM_Base_Start_IT(&htim6);
		HAL_TIM_Base_Start_IT(&htim7);
		sprintf(lcdLine_1st_line, "REC:None");
		sprintf(lcdLine_2st_line, "ADC:None");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			radio_mode_set(handle, RX_MODE);	
			ret = radio_buf_recv(handle, rx, &len);
			i=5000;
			while(i--&&ret != RADIO_RET_OK)
				ret = radio_buf_recv(handle, rx, &len);
			if(ret == RADIO_RET_OK)
			{
				if(check()==1)
				{
					if(rx[5]==0xa0&&rx[6]==0x01)
					{
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
						sprintf(lcdLine_1st_line, "REC:OK");
						sprintf(lcdLine_2st_line, "ADC:%.1f V",value[1]);
						led();
						time_2s=0;
						i=5;
						while(i--)
							radio_buf_send(handle, cmd_send, sizeof(cmd_send));
					}
				}
			}
			if(time_2s>20)
			{
				sprintf(lcdLine_1st_line, "REC:None");
				sprintf(lcdLine_2st_line, "ADC:None");
			}
			if(time_100ms==1)
			{
				for(i=0;i<=1;i++)
				{
					HAL_ADC_Start(&hadc);
					HAL_ADC_PollForConversion(&hadc,100);
					value[i]=HAL_ADC_GetValue(&hadc)*3.3/4095;
				}
				OLED_ShowString(0, 0, (unsigned char *)lcdLine_1st_line, 16);
				OLED_ShowString(0, 2, (unsigned char *)lcdLine_2st_line, 16);
			}
    }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN 4 */
unsigned int check()
{
	unsigned int i,num=0;
	for(i=0;i<=4;i++)
	{
		if(rx[i]==cmd_send[i])
			num++;
		else ;
	}
	if(num==5)
		return 1;
	else
		return 0;
}
void led()
{
	if(value[1]<1.5)
	{
		cmd_send[5]=0x0B;
		cmd_send[6]=0x11;
	}
	if(value[1]>=1.5&&value[0]<2.5)
	{
		cmd_send[5]=0x0B;
		cmd_send[6]=0x12;
	}
	if(value[1]>=2.5)
	{
		cmd_send[5]=0x0B;
		cmd_send[6]=0x13;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Instance==TIM6)
	 {
			time_i++;
			if(time_i==9999)		time_i=0;
			if(time_i%10==0)		time_10ms=!time_10ms;
			if(time_i%100==0)		
			{
				time_100ms=!time_100ms;
				time_2s++;
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
