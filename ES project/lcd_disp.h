#ifndef lcd_disp_h
#define lcd_disp_h

#include <LPC17xx.h>
#include <stdlib.h>
void lcd_init(void);
void write(int,int);
void lcd_comdata(int,int);
void clear_ports(void);
void lcd_puts(unsigned char*);

void initimer0(int sec){
	LPC_TIM0 -> CTCR=0x0;
	LPC_TIM0 -> TCR=0x2;
	LPC_TIM0 -> PR=299999;
	LPC_TIM0->MR0=sec;
	LPC_TIM0->EMR=0X20;
}
void delay_clock(){
	LPC_TIM0 -> TCR=0x2;
	LPC_TIM0 -> TCR=0x1;
	while(!(LPC_TIM0 -> EMR&1));
		LPC_TIM0 -> TCR=0x0;
}
void initimer1(){
	LPC_TIM1 -> CTCR=0x0;
	LPC_TIM1 -> TCR=0x2;
	LPC_TIM1 -> PR=2999;
}
void delay_lcd(int millisecond){
	LPC_TIM1 -> TCR=0x2;
	LPC_TIM1 -> TCR=0x1;
	while(LPC_TIM1 -> TC < millisecond);
	LPC_TIM1 -> TCR=0x0;
}
void lcd_init()
{
	LPC_PINCON->PINSEL1&=0xFC003FFF;
	LPC_GPIO0->FIODIR|=0x0F<<23|1<<27|1<<28;
	clear_ports();
	delay_lcd(320);
	lcd_comdata(0x33,0);
	delay_lcd(300);
	lcd_comdata(0x32,0);
	delay_lcd(300);
	lcd_comdata(0x28,0);
	delay_lcd(300);
	lcd_comdata(0x0c,0);//display on cursor off
	delay_lcd(80);
	lcd_comdata(0x06,0);
	delay_lcd(80);
	lcd_comdata(0x01,0);//display clear
	delay_lcd(100);
	return;
}
void lcd_comdata(int temp1,int type)
{
	int temp2=temp1 & 0xf0;
	temp2=temp2<<19;
	write(temp2,type);
	temp2=temp1 & 0x0f;
	temp2=temp2<<23;
	write(temp2,type);
	delay_lcd(100);
	return;
}
void write(int temp2, int type){ 
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2; // Assign the value to the data lines 
    if(type==0)
        LPC_GPIO0->FIOCLR = 1<<27; // clear bit RS for Command
    else
        LPC_GPIO0->FIOSET = 1<<27; // set bit RS for Data
    LPC_GPIO0->FIOSET = 1<<28; // EN=1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = 1<<28; // EN =0
    return;
}
void clear_ports(void) {
    /* Clearing the lines at power on */
    LPC_GPIO0->FIOCLR = 0x0F<<23; //Clearing data lines
    LPC_GPIO0->FIOCLR = 1<<27; //Clearing RS line
    LPC_GPIO0->FIOCLR = 1<<28; //Clearing Enable line
    
    return;
}
void lcd_puts(unsigned char *buf1) {
    unsigned int i=0;
    unsigned int temp3;
    while(buf1[i]!='\0') {
        temp3 = buf1[i];
        lcd_comdata(temp3, 1);
        i++;
        if(i==16) {
            lcd_comdata(0xc0, 0);
        }
    }
    return;
}
#endif