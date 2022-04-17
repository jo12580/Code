#include"key.h"

uint8_t count;
uint8_t Row_Scan(void)
{
	uint8_t key;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1)
	{
		count++;
		if(count==5)
		{
			key=2;
			count=0;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1)
	{
		count++;
		if(count==5)
		{
			key=1;
			count=0;
		}
	}
//	else key=0;
	return key;
}

unsigned int key2(void)
{
	uint8_t value;
	unsigned int key=0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	value=Row_Scan();
	if(value!=0)
	{
		switch(value)
		{
			case 1:key= 11;break;
			case 2:key= 21;break;
			case 0:key= 0 ;break;
//			default: break;
		}
	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	value=Row_Scan();
	if(value!=0)
	{
		switch(value)
		{
			case 1:key= 12;break;
			case 2:key= 22;break;
			case 0:key= 0 ;break;
//			default: break;
		}
	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	value=Row_Scan();
	if(value!=0)
	{
		switch(value)
		{
			case 1:key= 13;break;
			case 2:key= 23;break;
			case 0:key= 0 ;break;
//			default: break;
		}
	}
	return (key);
}

////////////////////////////////////////////////////////////////////////
//unsigned int i;		//可用
//unsigned int key2(void)
//{
//	unsigned int key_i=0;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1) 
//		{
//			i++;
//			if(i==1)
//			{
//				key_i=11;
//				i=0;
//			}
//		}
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1) 
//		{
//			i++;
//			if(i==1)
//			{
//				key_i=21;
//				i=0;
//			}
////			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
//		}
//		
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) 
//		{
//			i++;
//			if(i==1)
//			{
//				key_i=12;
//				i=0;
//			}
//		}
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) 
//		{
//			i++;
//			if(i==1)
//			{
//				key_i=22;
//				i=0;
//			}
//		}
//		
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) 
//		{
//			i++;
//			if(i==1)
//			{
//				key_i=13;
//				i=0;
//			}
//		}
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) 
//		{
//			i++;
//			if(i==1)
//			{
//				key_i=23;
//				i=0;
//			}
//		}
////	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//	return key_i;
//}
//////////////////////////////////////////////////////////////////////////

//unsigned int key2(void)
//{
//	unsigned int key_i=0;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1) 
//		{
////			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1)
//				key_i=11;
//		}
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1) 
//		{
////			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1)
//				key_i=21;
//		}
//		
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1) 
//		{
////			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1)
//				key_i=12;
//		}
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1) 
//		{
////			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1)
//				key_i=22;
//		}
//		
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1) 
//		{
////			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1)
//				key_i=13;
//		}
//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1) 
//		{
////			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1)
//				key_i=23;
//		}
//	return (key_i);
//}

//////////////////////////////////////////////////////////////////////////////
//uint8_t count = 0;
//uint8_t Row_Scan(void)
//{
//    uint8_t Row1st_Value = 0;
//    uint8_t Row2nd_Value = 0;

//    uint8_t RowValue_Return = 0;

//    Row2nd_Value = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7);
//    Row1st_Value = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);


//    if (Row1st_Value == 1)
//    {
//        count++;
//        if (count == 3)        //5次检测均检测到输入，认定为有输入
//        {
//            count = 0;
//            RowValue_Return = 1;
//        }
//    }

//    if (Row2nd_Value == 1)
//    {
//        count++;
//        if (count == 3)        //5次检测均检测到输入，认定为有输入
//        {
//            count = 0;
//            RowValue_Return = 2;
//        }
//    }

//    return (RowValue_Return);
//}

//unsigned int key2(void)
//{
//    unsigned int Key_Value = 0;
//    uint8_t key = 0;

//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
////    COLUMN3rd_WRITE_LOW;
////    COLUMN1st_WRITE_HIGH;
//    key = Row_Scan();
//    if (key != 0)
//    {
//        switch (key)
//        {
//        case 1:
//            Key_Value = 11;
//            break;
//        case 2:
//            Key_Value = 21;
//            break;
//        default:
//            break;
//        }
//    }
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
////    COLUMN1st_WRITE_LOW;

////    COLUMN2nd_WRITE_HIGH;
//    key = Row_Scan();
//    if (key != 0)
//    {
//        switch (key)
//        {
//        case 1:
//            Key_Value = 12;
//            break;
//        case 2:
//            Key_Value = 22;
//            break;
//        default:
//            break;
//        }
//    }
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
////    COLUMN2nd_WRITE_LOW;

////    COLUMN3rd_WRITE_HIGH;
//    key = Row_Scan();
//    if (key != 0)
//    {
//        switch (key)
//        {
//        case 1:
//            Key_Value = 13;
//            break;
//        case 2:
//            Key_Value = 23;
//            break;
//        default:
//            break;
//        }
//    }
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

//    return (Key_Value);
//}

//void Keyboard_Init(void)
//{

//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

//}
