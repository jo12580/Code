#ifndef __DRIVE_H__
#define __DRIVE_H__

typedef unsigned int u16;
typedef unsigned char u8;

void TurnRight1();
void TurnRight2()	;
void TurnLeft1();
void TurnLeft2();
void Forward()	;
void Backward();
void Stop()	;

void Timer0Init();
void Timer1Init();
void XunJi();
void delay(u16 i);

#endif