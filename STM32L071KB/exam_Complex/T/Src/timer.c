#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"



TIM_HandleTypeDef    TimHandle;
/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;





//延时 nms，精确延时
//nms:要延时的 ms 数
void delay_ms(uint32_t nms)
{
    HAL_Delay(nms);
}
void TIM2_Init(void)
{
    /* Compute the prescaler value to have TIMx counter clock equal to 1000 Hz */
    uwPrescalerValue = (uint32_t)(2000000 / 1000) - 1;

    /* Set TIMx instance */
    TimHandle.Instance = TIM2;

    /* Initialize TIMx peripheral as follows:
         + Period = 100 - 1
         + Prescaler = (SystemCoreClock/1) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    TimHandle.Init.Period            = 10 - 1;
    TimHandle.Init.Prescaler         = uwPrescalerValue;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&TimHandle);

    HAL_TIM_Base_Start_IT(&TimHandle);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}





