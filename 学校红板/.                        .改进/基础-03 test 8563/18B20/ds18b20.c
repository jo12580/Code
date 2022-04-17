#include "ds18b20.h"	//����ͷ�ļ�
#include "delay.h"
#include "lcd1602.h"

uint temperature;

/*---------------�������߲�������-----------------------------------------

*  �������߹��������е�Э�����£�
*  ��ʼ����ROM��������洢����������������ݡ�
*  ÿ��Ҫ�Դ洢������ǰ���������Ƚ��г�ʼ����Ȼ�����ROM�����������ܶ�
*  �洢�����в�����
------------------------------------------------------------------------*/

/*************************************************************************
* ��        �� ��void ds18b20_reset(void)
* ��        �� ����ʼ��DS18B20
* ��        �� ��NULL
* ȫ �� ��  �� ��NULL  
* ��   ��   ֵ ��NULL
* ��        �� ��NULL
* �� д ��  �� ��2008-7-23 
* ����޸����� ��2008-7-23
*************************************************************************/
uchar ds18b20_reset(void)		   //��λ����
{
	uchar x=0;

	DQ=1;    			
	delay_time(2);    	//������ʱ
	DQ=0;    			//��Ƭ����DQ����  
	delay_time(80);   	//��ȷ��ʱ ���� 480us
	DQ=1;    			//��������
	delay_time(5);
	x=DQ;
	while(!DQ);
	delay_time(40);   	
	return x;
}

uchar ds18b20_read_slot()	 //��һλ
{
	uchar bdat;

	DQ=1;
	DQ=0;    		  
	DQ=1;    		
	bdat=DQ;
	delay_time(4);
	return bdat;
}

uchar  ds18b20_read_byte(void)		//��һ���ֽں���
{
	uchar i,dat;
	
	dat=0;
	for(i=0;i<8;i++)	dat|=(ds18b20_read_slot()<<i);
	return dat;
}

void ds18b20_write_slot(uchar bdat)		 //дһλ
{
	DQ=1;
	DQ=0;    		  
	DQ=bdat;    		
	delay_time(4);
	DQ=1;
}

void ds18b20_write_byte(uchar dat)//дһ���ֽں��� 0011 0011
{
	uchar i;
	
	for(i=0;i<8;i++)	ds18b20_write_slot(dat&(1<<i));
}

void ds18b20_set_resolution(uchar res)		//�ֱ������ú���
{
	while(ds18b20_reset());  				// ��ʼ��
	ds18b20_write_byte(skip_rom); 			// ����������кŵĲ���
	ds18b20_write_byte(write_scrctchpad); 	// д�ݴ���
	ds18b20_write_byte(0x1e); 				// ��������
	ds18b20_write_byte(0x0a); 				// ��������
	ds18b20_write_byte((res<<5)|0x1f); 		// �ֱ���
}

/*************************************************************************
* ��        �� ��int read_temperature(void)
* ��        �� ����ʼ�����޸�18b20��ʱ������
* ��        �� ��NULL
* ȫ �� ��  �� ��NULL  
* ��   ��   ֵ ��NULL
* ��        �� ��NULL
* �� д ��  �� ��2008-7-23 
* ����޸����� ��2008-7-23
*************************************************************************/
void ds18b20_read_temperature(void)
{
	uchar  a,b;

	while(ds18b20_reset());  				// ��ʼ��
	ds18b20_write_byte(skip_rom); 			// ����������кŵĲ���
	ds18b20_write_byte(convert_t);		 	// �����¶�ת��
	
	while(!ds18b20_read_slot());
		
	while(ds18b20_reset());  				// ��ʼ��
	ds18b20_write_byte(skip_rom); 			// ����������кŵĲ���
	ds18b20_write_byte(read_scrctchpad); 	// ���ݴ���

	a=ds18b20_read_byte();
	b=ds18b20_read_byte();
	temperature=b;
	temperature<<=8;
	temperature|=a;
}

/*----------------------------
   �¶���ʾ�ӳ���
-----------------------------*/
void ds18b20_display_temperature(void)
{
    uchar i,j;

	lcd1602_locate(8,1);						    //����λ
    if(temperature&0x80==0) lcd1602_write_data('+');
	else 
	{
		temperature=~temperature;
		temperature++;
		lcd1602_write_data('-');
	}

	i=(uchar)(temperature>>4); 						 //��������
    j=(uchar)((temperature&0x000f)*6.25);	         //С������

	lcd1602_write_data(i/10+0x30);
	lcd1602_write_data(i%10+0x30);
    lcd1602_locate(12,1);
	lcd1602_write_data(j/10+0x30);
	lcd1602_write_data(j%10+0x30);
}