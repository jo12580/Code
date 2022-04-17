
//说明：4X4 矩阵接P1口
//按键扫描函数，返回扫描键值
//------ 键盘扫描函数，使用行列反转扫描法

#include <reg52.h>
#include <jianpan.h>


void KeyDelay1Ms(int z)
 {
  int x,y;
  for (x=z;x>0;x--)
    for(y=110;y>0;y--);
 }



unsigned char KeyScan() 
{
  unsigned char cord_h,cord_l;// 
  P1=0x0f;            // 
  cord_h=P1&0x0f;     // 
 if(cord_h!=0x0f)    // 
 {
   KeyDelay1Ms(10);        //
  if((P1&0x0f)!=0x0f)
   {
    cord_h=P1&0x0f;  // 
    P1=cord_h|0xf0;  // 
    cord_l=P1&0xf0;  // 
    while((P1&0xf0)!=0xf0);// 
    return(cord_h+cord_l);//
   }
	  else
	 return(0xff);
	 
 } 
 else
	 return(0xff);
 
}

unsigned char KeyPro()
 {
  switch(KeyScan())
  {   
//------ 按下相应的键显示相对应的码值，此处“=”相当于确认键-----
//   case 0x7e:return '=';break;
//   case 0x7d:return '=';break;
//   case 0x7b:return '=';break;
//   case 0x77:return '=';break;
//   case 0xbe:return '=';break;
//   case 0xbd:return '3';break;
//   case 0xbb:return '6';break;
//   case 0xb7:return '9';break;
//   case 0xde:return '.';break;
//   case 0xdd:return '2';break;
//   case 0xdb:return '5';break;
//   case 0xd7:return '8';break;
//   case 0xee:return '0';break;
//   case 0xed:return '1';break;
//   case 0xeb:return '4';break;
//   case 0xe7:return '7';break;
//   default:return 0xff;break;		
		
		
	 case 0x7e:return '.';break;    // A种键盘
   case 0x7d:return '0';break;
   case 0x7b:return '#';break;
   case 0x77:return '=';break;
   case 0xbe:return '7';break;
   case 0xbd:return '8';break;
   case 0xbb:return '9';break;
   case 0xb7:return 'c';break;
   case 0xde:return '4';break;
   case 0xdd:return '5';break;
   case 0xdb:return '6';break;
   case 0xd7:return 'b';break;
   case 0xee:return '1';break;
   case 0xed:return '2';break;
   case 0xeb:return '3';break;
   case 0xe7:return 'a';break;
   default:return 0xff;break;	


//   case 0x7e:return 'a';break;   // B种键盘
//   case 0x7d:return 'b';break;
//   case 0x7b:return 'c';break;
//   case 0x77:return '=';break;
//   case 0xbe:return '3';break;
//   case 0xbd:return '6';break;
//   case 0xbb:return '9';break;
//   case 0xb7:return '#';break;
//   case 0xde:return '2';break;
//   case 0xdd:return '5';break;
//   case 0xdb:return '8';break;
//   case 0xd7:return '0';break;
//   case 0xee:return '1';break;
//   case 0xed:return '4';break;
//   case 0xeb:return '3';break;
//   case 0xe7:return '.';break;
//   default:return 0xff;break;	
  }
 }