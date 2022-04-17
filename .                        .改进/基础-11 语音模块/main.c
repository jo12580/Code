# include<reg52.h>
#include<voice.h>
sbit k1=P1^6;  //语音播放引脚定义 
sbit k2=P1^7;  //语音播放引脚定义 
sbit Led1=P1^4;
sbit Led2=P1^5;

void  delay(unsigned int i);

void main()
{
	unsigned char i;
	Led1=0;  Led2=1; P0=0; 	delay(100);	

	while (1)
		for(i=0;i<53;i++)
		{
			voice(i); 		//播放语音
			Led1=~Led1;  	//
			Led2=~Led2;		//	
		}
}
 

void  delay(unsigned int i)
{
	while(i--);
}

//语音素材列表
//地址	内容
//1		无声音
//2		0
//3		1
//4		2
//5		3
//6		4
//7		5
//8		6
//9		7
//10	8
//11	9
//12	十
//13	百
//14	点
//15	分
//16	秒
//17	年
//18	月
//19	日
//20	星期
//21	度
//22	百分之
//23	现在北京时间是
//24	现在温度是
//25	现在湿度是
//26	音乐（铁路广播前的提示音）
//27	整
//28	今天是
//29	上午
//30	下午
//31	晚上
//32	负
//33	滴（铃音，提示音）
//34	欢迎光临本店
//35	欢迎使用本公司产品
//36	欢迎再次光临
//37	广西师范大学
//38	电子工程学院
//39	电子科学与技术
//40	电子信息工程
//41	通信工程
//42	专业
//43	学号
//44	电子楼
//45	室
//46	大学生创新基地
//47	请设置参数
//48	请输入
//49	价格
//50	当前距离
//51	重量
//52	千克
