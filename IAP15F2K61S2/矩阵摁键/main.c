#include <reg52.H>
#include<Matrix_key.h>

void datapros() 	
{
			unsigned int a;
			a=key_scanf();
			switch(a)
			{
				case 4  : {P0=~a;}break;
				case 5  : {P0=~a;}break;
				case 6  : {P0=~a;}break;
				case 7  : {P0=~a;}break;
				
				case 8  : {P0=~a;}break;
				case 9  : {P0=~a;}break;
				case 10  :{P0=~a;}break;
				case 11  :{P0=~a;}break;

				case 12  : {P0=~a;}break;
				case 13  : {P0=~a;}break;
				case 14  : {P0=~a;}break;
				case 15  : {P0=~a;}break;
				
				case 16  : {P0=~a;}break;
				case 17  : {P0=~a;}break;
				case 18  : {P0=~a;}break;
				case 19  : {P0=~a;}break;			
			}			 
}

void main()
{
	  P2=0x80;
		while(1)
		{
			datapros();
		}
}