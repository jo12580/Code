//ΡΡ������С�����ṩ//
//�Ա���ַ��http://shop58277238.taobao.com//
//��ӭ����ʹ��//


/**************************Timer0*********************************************/

#include <REGX51.H>
#include <intrins.h>


#define RightSenser P1_4    //��Ѳ��
#define LeftSenser  P1_5    //��Ѳ��

unsigned char a,b,k,j;de;

void delay10ms()   //��ʱ����
  {
    for(a=100;a>0;a--)
      for(b=225;b>0;b--);
   }




//********************************************************

//********************************************************
main()
{
					delay10ms();
					delay10ms();
					delay10ms();
					delay10ms();



	while(1)		
	{
P1_0 = 1 ;
P1_1 = 1 ;
P1_2 = 1 ; 
P1_3 = 1; 
             	
	if((!RightSenser) && LeftSenser)   // ����ұ߼٣������
              	{
				
	         	P1_0 = 0 ;
                P1_1 = 1 ;
                P1_2 = 1 ; 
                P1_3 = 1; 

                    delay10ms();
					delay10ms();
            	}

   	if(RightSenser && (!LeftSenser))  // �����߼٣��ұ���
             	{
                	P1_2 = 0 ; 
                    P1_3 = 1; 
                    P1_0 = 1 ;
                    P1_1 = 1 ;			

	
					delay10ms();
					delay10ms();
                  	}

   	if((RightSenser) && (LeftSenser))  // �����߼٣��ұ���
             	{
               P1_0 = 0 ;
               P1_1 = 1 ;
               P1_2 = 0 ; 
               P1_3 = 1; 
            
	
					for (de=0; de<100; de++);  // ��ʱ

            	}
   	if((!RightSenser) && (!LeftSenser))  // �����߼٣��ұ���
             	{
               P1_0 = 0 ;
               P1_1 = 1 ;
               P1_2 = 1 ; 
               P1_3 = 0; 
            
	
			
					delay10ms();
					delay10ms();
					delay10ms();

            	}


	}
	}





