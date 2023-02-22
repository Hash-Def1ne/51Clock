#include <reg52.h>
#define uint unsigned int

sbit P07 = P0^7;

sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P32 = P3^2;
sbit P33 = P3^3;

sbit Button1 = P2^4;
sbit Button2 = P2^5;
sbit Button3 = P2^6;
sbit Button4 = P2^7;

uint num1,num2,num3,num4,second,timer0e,timer0ee,time = 254;

bit editmode,buttonstate1;

unsigned char code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void Delay(uint t){
	uint i,j;
	for(i=t;i>0;i--)
	for(j=114;j>0;j--);
}

void ShortDelay(uint t){
	uint i;
	for(i=t;i>0;i--);
}

void RefreshTime(){
	if(second >= 60) num4++,second = 0;
	if(num4 >= 10) num3++,num4 = 0;
	if(num3 >= 6 && num4 >= 0) num2++,num3 = 0,num4 = 0;
	if(num2 >= 10) num1++,num2 = 0,num3 = 0,num4 = 0;
	if(num1 >= 2 && num2 >= 4 && num3 >= 0 && num4 >= 0) num1 = 0,num2 = 0,num3 = 0,num4 = 0;
}

void Eint0() interrupt 0{
	EX0 = 0;
	num4++;
	RefreshTime();
}

void Eint1() interrupt 2{
	EX1 = 0;
	if(time == 255) time = 0;
	time++;
}

void Time0() interrupt 1{
	// second++;
	// RefreshTime();
	if(EX0 == 0 && P32 == 1){
		timer0e++;
		if(timer0e >= 255) timer0ee++,timer0e = 0;
		if(timer0ee >= 50) EX0 = 1,timer0ee = 0;
	}
	if(EX1 == 0 && P33 == 1) EX1 = 1;
}

void Displayclock(){
	P0 = table[num1];
	P20 = 0;
	ShortDelay(time);
	P20 = 1;
	ShortDelay(255 - time);

	P0 = table[num2];
	P07 = 0;
	P21 = 0;
	ShortDelay(time);
	P21 = 1;
	ShortDelay(255 - time);

	P0 = table[num3];
	P22 = 0;
	ShortDelay(time);
	P22 = 1;
	ShortDelay(255 - time);

	P0 = table[num4];
	P23 = 0;
	ShortDelay(time);
	P23 = 1;
	ShortDelay(255 - time);
}

void main(){
	TMOD = 0x22;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	EX0 = 1;
	IT0 = 1;
	EX1 = 1;
	IT1 = 1;
	P2 = 0x00;
	while(1){
		if(editmode == 0) Displayclock();
	}
}